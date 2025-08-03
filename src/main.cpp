#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    try {
        MainWindow window;
        window.show();
        
        return app.exec();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Application Error", 
                            QString("Failed to start application: %1").arg(e.what()));
        return -1;
    }
} 