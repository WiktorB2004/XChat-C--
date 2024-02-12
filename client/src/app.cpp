#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>

int chatWindow(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow mainWindow;

    // Load the image
    QPixmap backgroundImage("assets/images/background.jpg");

    // Create a QLabel to display the image as background
    QLabel *backgroundLabel = new QLabel(&mainWindow);
    backgroundLabel->setPixmap(backgroundImage);

    // Create a QWidget to hold the grid
    QWidget *gridWidget = new QWidget(&mainWindow);
    QGridLayout *gridLayout = new QGridLayout(gridWidget);

    QLabel *gridLabel = new QLabel(QString("(0, 0)"));
    gridLabel->setAlignment(Qt::AlignCenter);
    gridLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
    gridLayout->addWidget(gridLabel, 0, 0);

    // ----------------------

    QLineEdit *userInputLineEdit = new QLineEdit;
    QLabel *submitWidget = new QLabel;
    QPixmap submitImage("assets/images/submit.png");
    submitWidget->setPixmap(submitImage.scaled(QSize{25, 25}, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    // Create a horizontal layout for the form
    QHBoxLayout *formLayout = new QHBoxLayout;
    formLayout->addWidget(userInputLineEdit);
    formLayout->addWidget(submitWidget);

    QWidget *inputWidget = new QWidget();
    inputWidget->setLayout(formLayout);
    gridLayout->addWidget(inputWidget, 1, 0);

    // Create a vertical layout for left column
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    QLabel *leftColumnLabel = new QLabel("Left Column");
    leftColumnLabel->setAlignment(Qt::AlignCenter);
    leftColumnLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
    leftColumnLayout->addWidget(leftColumnLabel);

    // Set layout for the gridWidget
    gridWidget->setLayout(gridLayout);

    // Set layout for the rows widget
    QWidget *rowsWidget = new QWidget(&mainWindow);
    QHBoxLayout *rowsLayout = new QHBoxLayout(rowsWidget);
    rowsLayout->addLayout(leftColumnLayout);
    rowsLayout->addWidget(gridWidget);

    // Set central widget of the main window
    mainWindow.setCentralWidget(backgroundLabel);

    // Add the rows widget to the main window
    mainWindow.setCentralWidget(backgroundLabel);
    mainWindow.setCentralWidget(rowsWidget);

    mainWindow.setGeometry(
        QGuiApplication::primaryScreen()->geometry().center().x() - mainWindow.width() / 2,
        QGuiApplication::primaryScreen()->geometry().center().y() - mainWindow.height() / 2,
        mainWindow.width(),
        mainWindow.height());

    mainWindow.show();

    return a.exec();
}
