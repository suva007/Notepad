#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    // Creating shortcut ctrl + s
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    // Connect the shortcut to a slot (member function) that will handle the event
    connect(shortcut, &QShortcut::activated, this, &MainWindow::on_actionSave_as_triggered);

    // Creating shortcut ctrl + n
    QShortcut *shortcutNew = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this);
    // Connect the shortcut to a slot (member function) that will handle the event
    connect(shortcutNew, &QShortcut::activated, this, &MainWindow::on_actionNew_triggered);

    // Creating shortcut ctrl + o
    QShortcut *shortcutOpen = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this);
    // Connect the shortcut to a slot (member function) that will handle the event
    connect(shortcutOpen, &QShortcut::activated, this, &MainWindow::on_actionOpen_triggered);

    // Creating shortcut ctrl + q
    QShortcut *shortcutExit = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    // Connect the shortcut to a slot (member function) that will handle the event
    connect(shortcutExit, &QShortcut::activated, this, &MainWindow::on_actionExit_triggered);

    // Update status bar
    // Create and set the status bar
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    setCentralWidget(ui->textEdit);

    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateCursorPosition);

    // Call the functions to update the status bar initially
    updateCursorPosition();

    // To check if the file is saved or not.
    isSaved = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    // Before creating a new file, check if the old file does have some content.
    QTextCursor cursor = ui->textEdit->textCursor();
    int lineNumber = cursor.blockNumber() + 1;
    int columnNumber = cursor.columnNumber() + 1;

    if((lineNumber > 1 or columnNumber > 1) and !isSaved) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Unsaved Changes");
        msgBox.setText("Do you want to save the changes?");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int choice = msgBox.exec();
        switch (choice) {
            case QMessageBox::Save:
                // Save the changes
                on_actionSave_as_triggered();
                break;
            case QMessageBox::Discard:
                // Discard the changes
                currentFile.clear();
                break;
            case QMessageBox::Cancel:
                // Cancel the action
                return;
        }
    }
    // Continue with the desired action (e.g., closing the application)
    currentFile.clear();
    ui->textEdit->setText(QString()); //Clearing the content of textEdit for new file
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file"); // Dialogue for slecting and opening file
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    } //Read only file or not able to open a particular file
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    isSaved = true;
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        on_actionSave_as_triggered();
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
    isSaved = true;
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot Access printer");
        return;
    }
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow:: updateCursorPosition() {
    isSaved = false;
    QTextCursor cursor = ui->textEdit->textCursor();
    int lineNumber = cursor.blockNumber() + 1;
    int columnNumber = cursor.columnNumber() + 1;

    QString statusMessage = QString("Line: %1, Column: %2").arg(lineNumber).arg(columnNumber);
    statusBar->showMessage(statusMessage);
}
