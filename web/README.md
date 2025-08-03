# Qt Camera Web Application

A modern web-based camera application that provides the same functionality as the Qt C++ desktop version, but runs entirely in your browser using HTML5 WebRTC APIs.

## 🚀 Quick Start

### Windows
```cmd
cd web
run.bat
```

### Linux/macOS
```bash
cd web
chmod +x run.sh
./run.sh
```

### Manual Start
```bash
cd web
python3 server.py
```

The application will automatically open in your default browser at `http://localhost:8080`

## ✨ Features

### Core Functionality
- **Live Camera Feed**: Real-time video streaming using WebRTC
- **Playback Controls**: Start, Pause, Resume, Stop camera
- **Resolution Selection**: VGA (640x480), HD (1280x720), Full HD (1920x1080)
- **Multi-Camera Support**: Select from available camera devices
- **Real-time Monitoring**: Live frame rate and resolution display

### Enhanced Web Features
- **Screenshot Capture**: Take and download PNG screenshots
- **Video Recording**: Record and download WebM videos
- **Gallery View**: View captured screenshots with download options
- **Keyboard Shortcuts**: Spacebar (pause/resume), Arrow keys (simulate forward/rewind)
- **Responsive Design**: Works on desktop, tablet, and mobile devices
- **Error Handling**: User-friendly error messages and status updates

## 🎮 Controls

### Mouse Controls
- **Start Camera**: Begin live feed
- **Pause**: Freeze current frame
- **Resume**: Continue from paused state
- **Stop**: Stop camera and return to placeholder
- **Take Screenshot**: Capture current frame as PNG
- **Start/Stop Recording**: Record video as WebM file

### Keyboard Shortcuts
- **Spacebar**: Toggle pause/resume
- **Left Arrow**: Simulate rewind (demo)
- **Right Arrow**: Simulate forward (demo)

## 🔧 Requirements

### Browser Support
- **Chrome 60+** (Recommended)
- **Firefox 55+**
- **Safari 11+** (macOS)
- **Edge 79+**

### System Requirements
- **Python 3.6+** (for local server)
- **Camera device** (USB webcam or integrated camera)
- **Camera permissions** (browser will prompt)

### Network Requirements
- **Local server**: Works offline on `localhost`
- **HTTPS required**: For deployment on non-localhost domains

## 🛠️ Technical Details

### Architecture
- **Frontend**: Pure HTML5, CSS3, JavaScript (ES6+)
- **Camera API**: WebRTC getUserMedia API
- **Recording**: MediaRecorder API
- **Server**: Python 3 HTTP server
- **No Dependencies**: No external libraries or frameworks

### Browser APIs Used
- `navigator.mediaDevices.getUserMedia()` - Camera access
- `MediaRecorder` - Video recording
- `Canvas API` - Screenshot capture
- `Blob API` - File download functionality
- `URL.createObjectURL()` - File handling

### File Structure
```
web/
├── index.html          # Main application page
├── styles.css          # Responsive CSS styling
├── script.js           # Core application logic
├── server.py           # Local HTTP server
├── run.bat            # Windows launcher
├── run.sh             # Linux/macOS launcher
└── README.md          # This file
```

## 🚦 Status Indicators

### Visual Indicators
- 🔴 **Red**: Camera stopped/ready
- 🟢 **Green**: Camera running normally
- 🟡 **Yellow**: Camera paused
- 🔄 **Blue**: Settings being changed
- 📸 **Camera**: Screenshot captured
- 🎬 **Film**: Recording in progress

### Frame Rate Display
- Shows real-time FPS counter
- Updates every 100ms
- Displays 0 when paused or stopped

## 🔐 Privacy & Security

### Camera Access
- **Permission Required**: Browser prompts for camera access
- **Local Processing**: All video processing happens locally
- **No Network Upload**: Video never leaves your device
- **Secure Context**: Requires HTTPS or localhost

### Data Storage
- **No Server Storage**: Nothing is saved on the server
- **Local Downloads**: Screenshots/recordings save to your download folder
- **No Cookies**: No tracking or persistent storage
- **Session Only**: All data cleared when page is closed

## 🐛 Troubleshooting

### Camera Access Issues
```
Problem: "Camera access denied"
Solution: 
1. Click the camera icon in browser address bar
2. Select "Allow" for camera permissions
3. Refresh the page
```

### Performance Issues
```
Problem: Low frame rate or choppy video
Solution:
1. Close other applications using the camera
2. Lower the resolution setting
3. Close unnecessary browser tabs
4. Check system resources
```

### Browser Compatibility
```
Problem: Features not working
Solution:
1. Update to latest browser version
2. Enable camera permissions in browser settings
3. Try Chrome or Firefox for best compatibility
```

### Server Issues
```
Problem: "Port already in use"
Solution:
1. Stop other applications using port 8080
2. Edit server.py to use different port
3. Wait a few seconds and try again
```

## 🔧 Customization

### Change Server Port
Edit `server.py`:
```python
PORT = 8080  # Change to desired port
```

### Add New Resolutions
Edit `script.js`:
```javascript
// Add to resolution select options
<option value="3840x2160">4K UHD</option>
```

### Modify Styling
Edit `styles.css` for custom themes and layouts.

## 🌐 Deployment

### Local Development
- Use provided server scripts
- Access via `http://localhost:8080`
- No additional setup required

### Production Deployment
- **HTTPS Required**: Camera access needs secure context
- **SSL Certificate**: Required for non-localhost domains
- **CORS Headers**: Already configured in server.py
- **Static Hosting**: Can be served by any web server

### Example Apache/Nginx
```nginx
server {
    listen 443 ssl;
    server_name yourdomain.com;
    
    location / {
        root /path/to/web/files;
        index index.html;
    }
}
```

## 📱 Mobile Support

### Responsive Design
- Adapts to screen size automatically
- Touch-friendly controls
- Optimized for mobile cameras

### Mobile-Specific Features
- **Front/Rear Camera**: Automatic device selection
- **Touch Controls**: Tap-based interface
- **Portrait/Landscape**: Automatic orientation handling

## 🤝 Contributing

### Development Setup
1. Clone the repository
2. Navigate to `web/` directory
3. Start the development server
4. Make changes and test locally

### Code Style
- Use ES6+ JavaScript features
- Follow semantic HTML structure
- Use CSS Grid/Flexbox for layouts
- Comment complex functionality

## 📄 License

This web application is part of the Qt Camera Application project and follows the same MIT license terms.

---

**Note**: This web version provides the same core functionality as the Qt C++ desktop application but leverages modern web technologies for broader accessibility and easier deployment. 