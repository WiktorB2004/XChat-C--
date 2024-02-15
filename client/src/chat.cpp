#include "../include/app_windows.h"
#include "../include/custom_qelements.h"
#include "../include/server_connection.h"

ChatWindow::ChatWindow(ServerConnection *connection, QWidget *parent)
    : QMainWindow(parent), connection(connection)
{
    // Load the image
    QPixmap backgroundImage("assets/images/background.jpg");

    // Create a QLabel to display the image as background
    QLabel *backgroundLabel = new QLabel(this); // Note: Use "this" as the parent
    backgroundLabel->setPixmap(backgroundImage);

    // FIXME: FIX THE SIZING OF gridWidget
    ChatInput *gridWidget = new ChatInput(this, connection->client, this);

    // Create a vertical layout for left column
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    QLabel *leftColumnLabel = new QLabel("Left Column");
    leftColumnLabel->setAlignment(Qt::AlignCenter);
    leftColumnLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
    leftColumnLayout->addWidget(leftColumnLabel);

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
