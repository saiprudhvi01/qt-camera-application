#include "CameraController.h"
#include <QDebug>

CameraController::CameraController()
    : m_camera(std::make_unique<cv::VideoCapture>())
    , m_initialized(false)
    , m_running(false)
    , m_paused(false)
    , m_currentWidth(640)
    , m_currentHeight(480)
    , m_cameraIndex(0)
    , m_bufferIndex(0)
{
    m_frameBuffer.reserve(MAX_BUFFER_SIZE);
}

CameraController::~CameraController()
{
    stop();
}

void CameraController::initialize(int cameraIndex)
{
    if (m_initialized) {
        stop();
    }
    
    m_cameraIndex = cameraIndex;
    
    // Try to open the camera
    if (!m_camera->open(cameraIndex)) {
        throw CameraException("Failed to open camera with index " + std::to_string(cameraIndex));
    }
    
    // Verify camera is working
    cv::Mat testFrame;
    if (!m_camera->read(testFrame) || testFrame.empty()) {
        m_camera->release();
        throw CameraException("Camera opened but failed to capture test frame");
    }
    
    // Set initial resolution
    setResolution(m_currentWidth, m_currentHeight);
    
    m_initialized = true;
    m_running = false;
    m_paused = false;
    
    qDebug() << "Camera initialized successfully with index" << cameraIndex;
}

void CameraController::start()
{
    validateCamera();
    
    if (m_running) {
        return; // Already running
    }
    
    m_running = true;
    m_paused = false;
    
    // Clear frame buffer when starting
    m_frameBuffer.clear();
    m_bufferIndex = 0;
    
    qDebug() << "Camera started";
}

void CameraController::stop()
{
    if (!m_initialized) {
        return;
    }
    
    m_running = false;
    m_paused = false;
    
    if (m_camera && m_camera->isOpened()) {
        m_camera->release();
    }
    
    m_initialized = false;
    m_frameBuffer.clear();
    m_bufferIndex = 0;
    
    qDebug() << "Camera stopped";
}

void CameraController::pause()
{
    validateCamera();
    
    if (!m_running || m_paused) {
        return;
    }
    
    m_paused = true;
    
    // Store the current frame for display during pause
    if (!m_currentFrame.empty()) {
        m_pausedFrame = m_currentFrame.clone();
    }
    
    qDebug() << "Camera paused";
}

void CameraController::resume()
{
    validateCamera();
    
    if (!m_paused) {
        return;
    }
    
    m_paused = false;
    qDebug() << "Camera resumed";
}

void CameraController::setResolution(int width, int height)
{
    validateCamera();
    
    // Set the resolution
    m_camera->set(cv::CAP_PROP_FRAME_WIDTH, width);
    m_camera->set(cv::CAP_PROP_FRAME_HEIGHT, height);
    
    // Verify the resolution was set
    int actualWidth = static_cast<int>(m_camera->get(cv::CAP_PROP_FRAME_WIDTH));
    int actualHeight = static_cast<int>(m_camera->get(cv::CAP_PROP_FRAME_HEIGHT));
    
    m_currentWidth = actualWidth;
    m_currentHeight = actualHeight;
    
    qDebug() << "Resolution set to" << actualWidth << "x" << actualHeight;
    
    // Note: Some cameras may not support the exact resolution requested
    if (actualWidth != width || actualHeight != height) {
        qDebug() << "Warning: Requested resolution" << width << "x" << height 
                 << "but got" << actualWidth << "x" << actualHeight;
    }
}

std::pair<int, int> CameraController::getCurrentResolution() const
{
    return std::make_pair(m_currentWidth, m_currentHeight);
}

QPixmap CameraController::getCurrentFrame()
{
    validateCamera();
    
    if (!m_running) {
        return QPixmap(); // Return empty pixmap if not running
    }
    
    if (m_paused) {
        // Return the paused frame
        if (!m_pausedFrame.empty()) {
            return matToQPixmap(m_pausedFrame);
        }
        return QPixmap();
    }
    
    // Capture new frame
    cv::Mat frame = captureFrame();
    if (frame.empty()) {
        throw CameraException("Failed to capture frame from camera");
    }
    
    m_currentFrame = frame.clone();
    
    // Add to frame buffer for forward/rewind functionality
    if (m_frameBuffer.size() >= MAX_BUFFER_SIZE) {
        // Remove oldest frame
        m_frameBuffer.erase(m_frameBuffer.begin());
        m_bufferIndex = std::max(0, m_bufferIndex - 1);
    }
    m_frameBuffer.push_back(frame.clone());
    m_bufferIndex = m_frameBuffer.size() - 1;
    
    return matToQPixmap(frame);
}

void CameraController::skipFrames(int frameCount)
{
    validateCamera();
    
    if (frameCount > 0) {
        // Forward: Skip frames by reading and discarding them
        for (int i = 0; i < frameCount; ++i) {
            cv::Mat dummyFrame;
            if (!m_camera->read(dummyFrame)) {
                break; // Can't read more frames
            }
        }
        qDebug() << "Skipped" << frameCount << "frames forward";
    }
    else if (frameCount < 0) {
        // Backward: Use frame buffer if available
        int skipCount = -frameCount;
        if (!m_frameBuffer.empty() && m_bufferIndex >= skipCount) {
            m_bufferIndex -= skipCount;
            m_currentFrame = m_frameBuffer[m_bufferIndex].clone();
            qDebug() << "Skipped" << skipCount << "frames backward using buffer";
        }
        else {
            qDebug() << "Cannot skip backward: insufficient frame buffer";
            // Note: For a live camera feed, true backward skipping is not possible
            // This implementation uses a limited frame buffer
        }
    }
}

cv::Mat CameraController::captureFrame()
{
    cv::Mat frame;
    
    if (!m_camera->read(frame)) {
        throw CameraException("Failed to read frame from camera");
    }
    
    return frame;
}

QImage CameraController::matToQImage(const cv::Mat& mat)
{
    if (mat.empty()) {
        return QImage();
    }
    
    switch (mat.type()) {
        case CV_8UC1: {
            // Grayscale
            QImage qimg(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return qimg.copy(); // Deep copy
        }
        case CV_8UC3: {
            // BGR to RGB
            cv::Mat rgbMat;
            cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
            QImage qimg(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888);
            return qimg.copy(); // Deep copy
        }
        case CV_8UC4: {
            // BGRA to RGBA
            cv::Mat rgbaMat;
            cv::cvtColor(mat, rgbaMat, cv::COLOR_BGRA2RGBA);
            QImage qimg(rgbaMat.data, rgbaMat.cols, rgbaMat.rows, rgbaMat.step, QImage::Format_RGBA8888);
            return qimg.copy(); // Deep copy
        }
        default:
            qDebug() << "Unsupported cv::Mat format:" << mat.type();
            return QImage();
    }
}

QPixmap CameraController::matToQPixmap(const cv::Mat& mat)
{
    QImage qimg = matToQImage(mat);
    if (qimg.isNull()) {
        return QPixmap();
    }
    return QPixmap::fromImage(qimg);
}

void CameraController::validateCamera() const
{
    if (!m_initialized) {
        throw CameraException("Camera is not initialized");
    }
    
    if (!m_camera || !m_camera->isOpened()) {
        throw CameraException("Camera is not opened");
    }
} 