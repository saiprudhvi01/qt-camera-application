#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QTimer>
#include <QMessageBox>
#include <memory>

#include "CameraController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onPauseClicked();
    void onResumeClicked();
    void onForwardClicked();
    void onRewindClicked();
    void onResolutionChanged(int index);
    void updateFrame();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void connectSignals();
    void updateControlsState();
    void showErrorMessage(const QString& message);

    // UI Components
    QWidget* m_centralWidget;
    QLabel* m_cameraLabel;
    QPushButton* m_playButton;
    QPushButton* m_pauseButton;
    QPushButton* m_resumeButton;
    QPushButton* m_forwardButton;
    QPushButton* m_rewindButton;
    QComboBox* m_resolutionCombo;
    QLabel* m_currentResolutionLabel;
    QGroupBox* m_controlsGroup;
    QGroupBox* m_settingsGroup;
    
    // Camera and timer
    std::unique_ptr<CameraController> m_cameraController;
    QTimer* m_frameTimer;
    
    // Resolution options
    struct Resolution {
        int width;
        int height;
        QString name;
    };
    std::vector<Resolution> m_resolutions;
};

#endif // MAINWINDOW_H 