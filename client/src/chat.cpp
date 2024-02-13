#include "../include/app_windows.h"

ChatWindow::ChatWindow(QWidget *parent) : QMainWindow(parent)
{
    // Load the image
    QPixmap backgroundImage("assets/images/background.jpg");

    // Create a QLabel to display the image as background
    QLabel *backgroundLabel = new QLabel(this); // Note: Use "this" as the parent
    backgroundLabel->setPixmap(backgroundImage);

    // Create a QWidget to hold the grid
    QWidget *gridWidget = new QWidget(this); // Note: Use "this" as the parent
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
    QWidget *rowsWidget = new QWidget(this);
    QHBoxLayout *rowsLayout = new QHBoxLayout(rowsWidget);
    rowsLayout->addLayout(leftColumnLayout);
    rowsLayout->addWidget(gridWidget);

    // Set central widget of the main window
    setCentralWidget(backgroundLabel);

    // Add the rows widget to the main window
    setCentralWidget(backgroundLabel);
    setCentralWidget(rowsWidget);

    // Center the window on the screen
    setGeometry(
        QGuiApplication::primaryScreen()->geometry().center().x() - width() / 2,
        QGuiApplication::primaryScreen()->geometry().center().y() - height() / 2,
        width(),
        height());
}

ChatWindow::~ChatWindow()
{
    // Destructor implementation
}
