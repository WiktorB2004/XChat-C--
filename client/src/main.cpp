#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("XChat - Login");

    // Create a QLabel to hold the background image
    QLabel *backgroundLabel = new QLabel(&mainWindow);
    backgroundLabel->setScaledContents(true); // Ensure the image scales to fit the label

    // Set the background image
    QPixmap backgroundImage("assets/images/X.jpg");

    // Set a fixed size for the background label to ensure it fills the entire window
    backgroundLabel->setPixmap(backgroundImage.scaled(QSize{400, 400}, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // LOGIN FORM
    QLabel *serverIpLabel = new QLabel("Server ip:");
    QLineEdit *serverIpLineEdit = new QLineEdit;
    QLabel *usernameLabel = new QLabel("Username:");
    QLineEdit *usernameLineEdit = new QLineEdit;
    QLabel *passwordLabel = new QLabel("Password:");
    QLineEdit *passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Create a vertical layout for the form
    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->addWidget(serverIpLabel);
    formLayout->addWidget(serverIpLineEdit);
    formLayout->addWidget(usernameLabel);
    formLayout->addWidget(usernameLineEdit);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordLineEdit);

    QLabel *submitWidget = new QLabel;
    QPixmap submitImage("assets/images/submit.png");
    submitWidget->setPixmap(submitImage.scaled(QSize{50, 50}, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    QHBoxLayout *formMenuLayout = new QHBoxLayout;
    formMenuLayout->addStretch();
    formMenuLayout->addWidget(submitWidget);
    formMenuLayout->addStretch();

    QWidget *formMenuWidget = new QWidget;
    formMenuWidget->setLayout(formMenuLayout);

    // Create a widget to hold the form layout
    QWidget *formWidget = new QWidget;
    formWidget->setLayout(formLayout);

    // Create a horizontal layout to center the form
    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addWidget(formWidget);
    centerLayout->addWidget(formMenuWidget);

    // Set layout for main window
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(backgroundLabel);
    mainLayout->addLayout(centerLayout);

    // Set the main layout for the central widget of the main window
    QWidget *centralWidget = new QWidget(&mainWindow);
    centralWidget->setLayout(mainLayout);
    mainWindow.setCentralWidget(centralWidget);

    // Center the window on the screen
    mainWindow.setGeometry(
        QGuiApplication::primaryScreen()->geometry().center().x() - mainWindow.width() / 2,
        QGuiApplication::primaryScreen()->geometry().center().y() - mainWindow.height() / 2,
        mainWindow.width(),
        mainWindow.height());

    // Display the window
    mainWindow.show();

    return app.exec();
}
