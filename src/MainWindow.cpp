#include "MainWindow.h"
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_cameraLabel(nullptr)
    , m_playButton(nullptr)
    , m_pauseButton(nullptr)
    , m_resumeButton(nullptr)
    , m_forwardButton(nullptr)
    , m_rewindButton(nullptr)
    , m_resolutionCombo(nullptr)
    , m_currentResolutionLabel(nullptr)
    , m_controlsGroup(nullptr)
    , m_settingsGroup(nullptr)
    , m_cameraController(std::make_unique<CameraController>())
    , m_frameTimer(new QTimer(this))
{
    // Initialize resolution options
    m_resolutions = {
        {640, 480, "640x480 (VGA)"},
        {1280, 720, "1280x720 (HD)"},
        {1920, 1080, "1920x1080 (Full HD)"}
    };

    setupUI();
    setupMenuBar();
    setupStatusBar();
    connectSignals();
    
    // Set window properties
    setWindowTitle("Qt Camera Application");
    setMinimumSize(800, 600);
    resize(1000, 750);
    
    // Center window on screen
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
    
    // Initialize camera with default resolution
    try {
        m_cameraController->initialize(0); // Default camera index
        m_cameraController->setResolution(640, 480);
        updateControlsState();
        statusBar()->showMessage("Camera initialized successfully", 3000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to initialize camera: %1").arg(e.what()));
    }
}

MainWindow::~MainWindow()
{
    if (m_frameTimer->isActive()) {
        m_frameTimer->stop();
    }
    if (m_cameraController) {
        m_cameraController->stop();
    }
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Camera display area
    m_cameraLabel = new QLabel(this);
    m_cameraLabel->setMinimumSize(640, 480);
    m_cameraLabel->setStyleSheet("QLabel { background-color: black; border: 2px solid gray; }");
    m_cameraLabel->setAlignment(Qt::AlignCenter);
    m_cameraLabel->setText("Camera feed will appear here");
    m_cameraLabel->setScaledContents(true);
    
    // Controls group
    m_controlsGroup = new QGroupBox("Playback Controls", this);
    QHBoxLayout* controlsLayout = new QHBoxLayout(m_controlsGroup);
    
    m_playButton = new QPushButton("Play", this);
    m_pauseButton = new QPushButton("Pause", this);
    m_resumeButton = new QPushButton("Resume", this);
    m_forwardButton = new QPushButton("Forward", this);
    m_rewindButton = new QPushButton("Rewind", this);
    
    // Style buttons
    QString buttonStyle = "QPushButton { "
                         "min-width: 80px; "
                         "min-height: 30px; "
                         "font-size: 12px; "
                         "font-weight: bold; "
                         "} "
                         "QPushButton:disabled { "
                         "color: gray; "
                         "}";
    
    m_playButton->setStyleSheet(buttonStyle);
    m_pauseButton->setStyleSheet(buttonStyle);
    m_resumeButton->setStyleSheet(buttonStyle);
    m_forwardButton->setStyleSheet(buttonStyle);
    m_rewindButton->setStyleSheet(buttonStyle);
    
    controlsLayout->addWidget(m_playButton);
    controlsLayout->addWidget(m_pauseButton);
    controlsLayout->addWidget(m_resumeButton);
    controlsLayout->addWidget(m_forwardButton);
    controlsLayout->addWidget(m_rewindButton);
    controlsLayout->addStretch();
    
    // Settings group
    m_settingsGroup = new QGroupBox("Camera Settings", this);
    QHBoxLayout* settingsLayout = new QHBoxLayout(m_settingsGroup);
    
    settingsLayout->addWidget(new QLabel("Resolution:", this));
    
    m_resolutionCombo = new QComboBox(this);
    for (const auto& res : m_resolutions) {
        m_resolutionCombo->addItem(res.name);
    }
    m_resolutionCombo->setCurrentIndex(0);
    
    m_currentResolutionLabel = new QLabel("Current: 640x480", this);
    m_currentResolutionLabel->setStyleSheet("QLabel { font-weight: bold; color: blue; }");
    
    settingsLayout->addWidget(m_resolutionCombo);
    settingsLayout->addWidget(m_currentResolutionLabel);
    settingsLayout->addStretch();
    
    // Add to main layout
    mainLayout->addWidget(m_cameraLabel, 1);
    mainLayout->addWidget(m_controlsGroup);
    mainLayout->addWidget(m_settingsGroup);
}

void MainWindow::setupMenuBar()
{
    QMenuBar* menuBar = this->menuBar();
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Help menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About Qt Camera App",
                          "Qt Camera Application v1.0\n\n"
                          "A live camera feed application with playback controls\n"
                          "Built with Qt6 and OpenCV");
    });
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Ready");
}

void MainWindow::connectSignals()
{
    // Button connections
    connect(m_playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(m_pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(m_resumeButton, &QPushButton::clicked, this, &MainWindow::onResumeClicked);
    connect(m_forwardButton, &QPushButton::clicked, this, &MainWindow::onForwardClicked);
    connect(m_rewindButton, &QPushButton::clicked, this, &MainWindow::onRewindClicked);
    
    // Resolution combo connection
    connect(m_resolutionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onResolutionChanged);
    
    // Frame timer connection
    connect(m_frameTimer, &QTimer::timeout, this, &MainWindow::updateFrame);
}

void MainWindow::onPlayClicked()
{
    try {
        m_cameraController->start();
        m_frameTimer->start(33); // ~30 FPS
        updateControlsState();
        statusBar()->showMessage("Camera started", 2000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to start camera: %1").arg(e.what()));
    }
}

void MainWindow::onPauseClicked()
{
    try {
        m_cameraController->pause();
        m_frameTimer->stop();
        updateControlsState();
        statusBar()->showMessage("Camera paused", 2000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to pause camera: %1").arg(e.what()));
    }
}

void MainWindow::onResumeClicked()
{
    try {
        m_cameraController->resume();
        m_frameTimer->start(33);
        updateControlsState();
        statusBar()->showMessage("Camera resumed", 2000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to resume camera: %1").arg(e.what()));
    }
}

void MainWindow::onForwardClicked()
{
    try {
        m_cameraController->skipFrames(10); // Skip 10 frames forward
        statusBar()->showMessage("Skipped forward", 1000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to skip forward: %1").arg(e.what()));
    }
}

void MainWindow::onRewindClicked()
{
    try {
        m_cameraController->skipFrames(-10); // Skip 10 frames backward
        statusBar()->showMessage("Skipped backward", 1000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to skip backward: %1").arg(e.what()));
    }
}

void MainWindow::onResolutionChanged(int index)
{
    if (index < 0 || index >= static_cast<int>(m_resolutions.size())) {
        return;
    }
    
    const auto& resolution = m_resolutions[index];
    
    try {
        bool wasRunning = m_cameraController->isRunning();
        
        if (wasRunning) {
            m_frameTimer->stop();
            m_cameraController->stop();
        }
        
        m_cameraController->setResolution(resolution.width, resolution.height);
        m_currentResolutionLabel->setText(QString("Current: %1x%2")
                                         .arg(resolution.width)
                                         .arg(resolution.height));
        
        if (wasRunning) {
            m_cameraController->start();
            m_frameTimer->start(33);
        }
        
        statusBar()->showMessage(QString("Resolution changed to %1").arg(resolution.name), 2000);
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Failed to change resolution: %1").arg(e.what()));
        // Revert combo box to previous selection
        // This is a simple approach; in production, you might want to track the last successful resolution
    }
}

void MainWindow::updateFrame()
{
    try {
        QPixmap frame = m_cameraController->getCurrentFrame();
        if (!frame.isNull()) {
            m_cameraLabel->setPixmap(frame);
        }
    }
    catch (const std::exception& e) {
        showErrorMessage(QString("Error updating frame: %1").arg(e.what()));
        m_frameTimer->stop();
        updateControlsState();
    }
}

void MainWindow::updateControlsState()
{
    bool isRunning = m_cameraController->isRunning();
    bool isPaused = m_cameraController->isPaused();
    bool isInitialized = m_cameraController->isInitialized();
    
    m_playButton->setEnabled(isInitialized && !isRunning);
    m_pauseButton->setEnabled(isRunning && !isPaused);
    m_resumeButton->setEnabled(isInitialized && isPaused);
    m_forwardButton->setEnabled(isInitialized);
    m_rewindButton->setEnabled(isInitialized);
    m_resolutionCombo->setEnabled(isInitialized && !isRunning);
}

void MainWindow::showErrorMessage(const QString& message)
{
    QMessageBox::critical(this, "Error", message);
    statusBar()->showMessage("Error occurred", 3000);
}

#include "MainWindow.moc" 