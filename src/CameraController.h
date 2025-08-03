#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QImage>
#include <memory>
#include <vector>
#include <stdexcept>

class CameraController
{
public:
    CameraController();
    ~CameraController();

    // Camera lifecycle
    void initialize(int cameraIndex = 0);
    void start();
    void stop();
    void pause();
    void resume();
    
    // Camera settings
    void setResolution(int width, int height);
    std::pair<int, int> getCurrentResolution() const;
    
    // Frame operations
    QPixmap getCurrentFrame();
    void skipFrames(int frameCount);
    
    // State queries
    bool isInitialized() const { return m_initialized; }
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }

private:
    cv::Mat captureFrame();
    QImage matToQImage(const cv::Mat& mat);
    QPixmap matToQPixmap(const cv::Mat& mat);
    void validateCamera() const;

    std::unique_ptr<cv::VideoCapture> m_camera;
    cv::Mat m_currentFrame;
    cv::Mat m_pausedFrame;
    
    bool m_initialized;
    bool m_running;
    bool m_paused;
    
    int m_currentWidth;
    int m_currentHeight;
    int m_cameraIndex;
    
    // Frame buffer for forward/rewind functionality
    std::vector<cv::Mat> m_frameBuffer;
    int m_bufferIndex;
    static const int MAX_BUFFER_SIZE = 100;
};

// Custom exception for camera errors
class CameraException : public std::runtime_error
{
public:
    explicit CameraException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // CAMERACONTROLLER_H 