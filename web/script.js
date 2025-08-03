class CameraApp {
    constructor() {
        this.stream = null;
        this.mediaRecorder = null;
        this.recordedChunks = [];
        this.isRecording = false;
        this.isPaused = false;
        this.frameRate = 0;
        this.frameCount = 0;
        this.lastFrameTime = Date.now();
        
        // Get DOM elements
        this.video = document.getElementById('cameraVideo');
        this.placeholder = document.getElementById('placeholderMessage');
        this.playBtn = document.getElementById('playBtn');
        this.pauseBtn = document.getElementById('pauseBtn');
        this.resumeBtn = document.getElementById('resumeBtn');
        this.stopBtn = document.getElementById('stopBtn');
        this.screenshotBtn = document.getElementById('screenshotBtn');
        this.recordBtn = document.getElementById('recordBtn');
        this.resolutionSelect = document.getElementById('resolutionSelect');
        this.deviceSelect = document.getElementById('deviceSelect');
        this.statusText = document.getElementById('statusText');
        this.statusIndicator = document.getElementById('statusIndicator');
        this.frameRateDisplay = document.getElementById('frameRate');
        this.resolutionDisplay = document.getElementById('resolution');
        this.currentResolution = document.getElementById('currentResolution');
        this.screenshotsSection = document.getElementById('screenshotsSection');
        this.screenshotsGallery = document.getElementById('screenshotsGallery');
        this.canvas = document.getElementById('screenshotCanvas');
        this.errorModal = document.getElementById('errorModal');
        this.errorMessage = document.getElementById('errorMessage');
        
        this.init();
    }
    
    async init() {
        this.bindEvents();
        await this.loadCameraDevices();
        this.updateStatus('Ready - Click "Start Camera" to begin', '🔴');
        this.startFrameRateCounter();
    }
    
    bindEvents() {
        this.playBtn.addEventListener('click', () => this.startCamera());
        this.pauseBtn.addEventListener('click', () => this.pauseCamera());
        this.resumeBtn.addEventListener('click', () => this.resumeCamera());
        this.stopBtn.addEventListener('click', () => this.stopCamera());
        this.screenshotBtn.addEventListener('click', () => this.takeScreenshot());
        this.recordBtn.addEventListener('click', () => this.toggleRecording());
        this.resolutionSelect.addEventListener('change', () => this.changeResolution());
        this.deviceSelect.addEventListener('change', () => this.changeDevice());
        
        // Error modal
        document.querySelector('.close').addEventListener('click', () => {
            this.errorModal.style.display = 'none';
        });
        
        window.addEventListener('click', (event) => {
            if (event.target === this.errorModal) {
                this.errorModal.style.display = 'none';
            }
        });
    }
    
    async loadCameraDevices() {
        try {
            const devices = await navigator.mediaDevices.enumerateDevices();
            const videoDevices = devices.filter(device => device.kind === 'videoinput');
            
            this.deviceSelect.innerHTML = '<option value="">Select camera...</option>';
            
            videoDevices.forEach((device, index) => {
                const option = document.createElement('option');
                option.value = device.deviceId;
                option.textContent = device.label || `Camera ${index + 1}`;
                this.deviceSelect.appendChild(option);
            });
            
            if (videoDevices.length > 0) {
                this.deviceSelect.value = videoDevices[0].deviceId;
            }
        } catch (error) {
            console.error('Error loading camera devices:', error);
            this.showError('Failed to load camera devices: ' + error.message);
        }
    }
    
    async startCamera() {
        try {
            const constraints = this.getConstraints();
            this.stream = await navigator.mediaDevices.getUserMedia(constraints);
            
            this.video.srcObject = this.stream;
            this.video.style.display = 'block';
            this.placeholder.style.display = 'none';
            
            this.updateStatus('Camera started - Live feed active', '🟢');
            this.updateButtonStates('running');
            this.updateResolutionDisplay();
            
            // Start frame rate monitoring
            this.frameCount = 0;
            this.lastFrameTime = Date.now();
            
        } catch (error) {
            console.error('Error starting camera:', error);
            this.showError('Failed to start camera: ' + error.message);
        }
    }
    
    pauseCamera() {
        if (this.stream) {
            const tracks = this.stream.getVideoTracks();
            tracks.forEach(track => track.enabled = false);
            
            this.isPaused = true;
            this.updateStatus('Camera paused - Feed frozen', '🟡');
            this.updateButtonStates('paused');
        }
    }
    
    resumeCamera() {
        if (this.stream) {
            const tracks = this.stream.getVideoTracks();
            tracks.forEach(track => track.enabled = true);
            
            this.isPaused = false;
            this.updateStatus('Camera resumed - Live feed active', '🟢');
            this.updateButtonStates('running');
        }
    }
    
    stopCamera() {
        if (this.stream) {
            const tracks = this.stream.getTracks();
            tracks.forEach(track => track.stop());
            this.stream = null;
        }
        
        if (this.mediaRecorder && this.isRecording) {
            this.mediaRecorder.stop();
        }
        
        this.video.style.display = 'none';
        this.placeholder.style.display = 'flex';
        this.isPaused = false;
        this.isRecording = false;
        
        this.updateStatus('Camera stopped', '🔴');
        this.updateButtonStates('stopped');
        this.frameRate = 0;
        this.updateFrameRateDisplay();
    }
    
    async changeResolution() {
        if (this.stream) {
            this.stopCamera();
            await new Promise(resolve => setTimeout(resolve, 100)); // Small delay
            this.startCamera();
        }
        
        const resolution = this.resolutionSelect.value;
        this.currentResolution.textContent = resolution;
        this.updateStatus(`Resolution changed to ${resolution}`, '🔄');
    }
    
    async changeDevice() {
        if (this.stream) {
            this.stopCamera();
            await new Promise(resolve => setTimeout(resolve, 100)); // Small delay
            this.startCamera();
        }
    }
    
    takeScreenshot() {
        if (!this.video.videoWidth || !this.video.videoHeight) {
            this.showError('No video frame available for screenshot');
            return;
        }
        
        const context = this.canvas.getContext('2d');
        this.canvas.width = this.video.videoWidth;
        this.canvas.height = this.video.videoHeight;
        
        context.drawImage(this.video, 0, 0);
        
        this.canvas.toBlob((blob) => {
            const timestamp = new Date().toLocaleString();
            this.addScreenshotToGallery(blob, timestamp);
            this.updateStatus('Screenshot captured', '📸');
        }, 'image/png');
    }
    
    addScreenshotToGallery(blob, timestamp) {
        const url = URL.createObjectURL(blob);
        
        const screenshotDiv = document.createElement('div');
        screenshotDiv.className = 'screenshot-item';
        
        const img = document.createElement('img');
        img.src = url;
        img.alt = `Screenshot ${timestamp}`;
        
        const downloadBtn = document.createElement('button');
        downloadBtn.className = 'download-btn';
        downloadBtn.textContent = '⬇️';
        downloadBtn.title = 'Download';
        downloadBtn.addEventListener('click', () => {
            const a = document.createElement('a');
            a.href = url;
            a.download = `screenshot_${timestamp.replace(/[:/]/g, '-')}.png`;
            a.click();
        });
        
        screenshotDiv.appendChild(img);
        screenshotDiv.appendChild(downloadBtn);
        this.screenshotsGallery.appendChild(screenshotDiv);
        
        this.screenshotsSection.style.display = 'block';
    }
    
    toggleRecording() {
        if (!this.isRecording) {
            this.startRecording();
        } else {
            this.stopRecording();
        }
    }
    
    startRecording() {
        if (!this.stream) {
            this.showError('No camera stream available for recording');
            return;
        }
        
        try {
            this.recordedChunks = [];
            this.mediaRecorder = new MediaRecorder(this.stream);
            
            this.mediaRecorder.ondataavailable = (event) => {
                if (event.data.size > 0) {
                    this.recordedChunks.push(event.data);
                }
            };
            
            this.mediaRecorder.onstop = () => {
                const blob = new Blob(this.recordedChunks, { type: 'video/webm' });
                const url = URL.createObjectURL(blob);
                
                const a = document.createElement('a');
                a.href = url;
                a.download = `recording_${new Date().toISOString().replace(/[:.]/g, '-')}.webm`;
                a.click();
                
                URL.revokeObjectURL(url);
                this.updateStatus('Recording saved', '💾');
            };
            
            this.mediaRecorder.start();
            this.isRecording = true;
            
            this.recordBtn.innerHTML = '<i class="icon">⏹️</i> Stop Recording';
            this.recordBtn.classList.add('danger');
            document.body.classList.add('recording');
            
            this.updateStatus('Recording started', '🔴');
            
        } catch (error) {
            console.error('Error starting recording:', error);
            this.showError('Failed to start recording: ' + error.message);
        }
    }
    
    stopRecording() {
        if (this.mediaRecorder && this.isRecording) {
            this.mediaRecorder.stop();
            this.isRecording = false;
            
            this.recordBtn.innerHTML = '<i class="icon">🎬</i> Start Recording';
            this.recordBtn.classList.remove('danger');
            document.body.classList.remove('recording');
            
            this.updateStatus('Recording stopped', '⏹️');
        }
    }
    
    getConstraints() {
        const resolution = this.resolutionSelect.value;
        const [width, height] = resolution.split('x').map(Number);
        const deviceId = this.deviceSelect.value;
        
        const constraints = {
            video: {
                width: { ideal: width },
                height: { ideal: height },
                frameRate: { ideal: 30 }
            },
            audio: false
        };
        
        if (deviceId) {
            constraints.video.deviceId = { exact: deviceId };
        }
        
        return constraints;
    }
    
    updateButtonStates(state) {
        switch (state) {
            case 'stopped':
                this.playBtn.disabled = false;
                this.pauseBtn.disabled = true;
                this.resumeBtn.disabled = true;
                this.stopBtn.disabled = true;
                this.screenshotBtn.disabled = true;
                this.recordBtn.disabled = true;
                break;
            case 'running':
                this.playBtn.disabled = true;
                this.pauseBtn.disabled = false;
                this.resumeBtn.disabled = true;
                this.stopBtn.disabled = false;
                this.screenshotBtn.disabled = false;
                this.recordBtn.disabled = false;
                break;
            case 'paused':
                this.playBtn.disabled = true;
                this.pauseBtn.disabled = true;
                this.resumeBtn.disabled = false;
                this.stopBtn.disabled = false;
                this.screenshotBtn.disabled = false;
                this.recordBtn.disabled = false;
                break;
        }
    }
    
    updateStatus(message, indicator) {
        this.statusText.textContent = message;
        this.statusIndicator.textContent = indicator;
    }
    
    updateResolutionDisplay() {
        if (this.video.videoWidth && this.video.videoHeight) {
            const resolution = `${this.video.videoWidth}x${this.video.videoHeight}`;
            this.resolutionDisplay.textContent = `Resolution: ${resolution}`;
        }
    }
    
    startFrameRateCounter() {
        setInterval(() => {
            if (this.stream && !this.isPaused) {
                this.frameCount++;
                const now = Date.now();
                const timeDiff = now - this.lastFrameTime;
                
                if (timeDiff >= 1000) {
                    this.frameRate = Math.round((this.frameCount * 1000) / timeDiff);
                    this.frameCount = 0;
                    this.lastFrameTime = now;
                }
            } else {
                this.frameRate = 0;
            }
            
            this.updateFrameRateDisplay();
        }, 100);
    }
    
    updateFrameRateDisplay() {
        this.frameRateDisplay.textContent = `FPS: ${this.frameRate}`;
    }
    
    showError(message) {
        this.errorMessage.textContent = message;
        this.errorModal.style.display = 'block';
        console.error('Camera App Error:', message);
    }
}

// Initialize the application when the page loads
document.addEventListener('DOMContentLoaded', () => {
    // Check for getUserMedia support
    if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
        alert('Your browser does not support camera access. Please use a modern browser like Chrome, Firefox, or Safari.');
        return;
    }
    
    // Check for HTTPS (required for camera access in modern browsers)
    if (location.protocol !== 'https:' && location.hostname !== 'localhost' && location.hostname !== '127.0.0.1') {
        alert('Camera access requires HTTPS. Please serve this page over HTTPS or run it locally.');
        return;
    }
    
    new CameraApp();
});

// Simulate forward/rewind functionality for demo purposes
document.addEventListener('keydown', (event) => {
    const app = window.cameraApp;
    if (!app) return;
    
    switch (event.key) {
        case 'ArrowRight':
            // Simulate forward skip
            if (app.stream) {
                app.updateStatus('Skipped forward 10 frames', '⏩');
                setTimeout(() => {
                    app.updateStatus('Camera running - Live feed active', '🟢');
                }, 1000);
            }
            break;
        case 'ArrowLeft':
            // Simulate rewind skip
            if (app.stream) {
                app.updateStatus('Skipped backward 10 frames', '⏪');
                setTimeout(() => {
                    app.updateStatus('Camera running - Live feed active', '🟢');
                }, 1000);
            }
            break;
        case ' ':
            // Space bar to pause/resume
            event.preventDefault();
            if (app.stream && !app.isPaused) {
                app.pauseCamera();
            } else if (app.stream && app.isPaused) {
                app.resumeCamera();
            }
            break;
    }
}); 