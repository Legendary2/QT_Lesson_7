#include "mainwindow.h"

#include "helpform.h"
#include "fileview.h"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDataStream>
#include <QTranslator>
#include <QLibraryInfo>
#include <QEvent>
#include <QClipboard>
#include <QShortcut>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QPrinter>
#include <QPrintDialog>
#include <QToolBar>
#include <QMenu>
#include <QPushButton>
#include <QTextDocumentFragment>
#include <QTextDocument>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    openedFileName("")
{
    ui->setupUi(this);
    Init();

    this->aDirList = new QFileInfoList();

    this->model = new QListViewExplorerModel();

    this->currentFolder = QDir(QDir::currentPath());

    this->model->getFolderList(this->currentFolder.absolutePath(),this->aDirList);


    this->ui->listView->setModel(model);

    this->ui->lineEdit->setText(QDir::currentPath());

    this->ui->listView->setWordWrap(true);

    tEdit = new QTextEdit(this);
    setCentralWidget(tEdit);
    QToolBar *tBar = addToolBar("Formatted");
    QMenu *menu = new QMenu(this);
    QAction *action = menu->addAction(tr("Random size of font"));
    connect(action, SIGNAL(triggered()), this, SLOT(randomSizeOfFont()));
    action = menu->addAction(tr("Set font"));
    connect(action, SIGNAL(triggered()), this, SLOT(setFont()));
    action = menu->addAction(tr("Convert to HTML"));
    connect(action, SIGNAL(triggered()), this, SLOT(convertToHTML()));
    action = menu->addAction(tr("Convert from HTML"));
    connect(action, SIGNAL(triggered()), this, SLOT(convertFromHTML()));
    QPushButton *button = new QPushButton(tr("Formatted menu"), this);
    tBar->addWidget(button);
    button->setMenu(menu);
    srand(clock());
}

MainWindow::~MainWindow(){

    delete ui;
}

void MainWindow::Init(){

    ui->textEdit->setReadOnly(true);
    ui->textEdit->setDisabled(true);

    QAction *exitAction = new QAction(tr("Exit"));
    exitAction->setShortcut(tr("CTRL+Q"));
    connect(exitAction, SIGNAL(triggered()),this, SLOT(close()));

}

void MainWindow::changeTranslator(QString postfix)
{
    QApplication::removeTranslator(translator);
    translator = new QTranslator(this);
    translator->load(QApplication::applicationName() + "_" + postfix);
    QApplication::installTranslator(translator);
}

void MainWindow::changeEvent(QEvent *event)
{
    setWindowTitle(tr("Editor"));

    if(event->type() == QEvent::LanguageChange){
        ui->menu->setTitle(tr("????????????????"));
        ui->actionEnglish->setText(tr("????????????????????"));
        ui->actionNew->setText(tr("??????????"));
        ui->actionOpen->setText(tr("??????????????"));
        ui->actionReadOnly->setText(tr("???????????? ????????????"));
        ui->actionClose->setText(tr("??????????????"));
        ui->actionSave->setText(tr("??????????????????"));
        ui->actionSaveAs->setText(tr("?????????????????? ??????"));
        ui->ActionExit->setText(tr("??????????"));
        ui->actionHelp->setText(tr("??????????????"));
        ui->actionRussian->setText(tr("??????????????"));

    }else
    {
        QMainWindow::changeEvent(event);
    }
}

void MainWindow::ExtendedFileSave(){

    if(!ui->textEdit->toPlainText().isEmpty())
    {
        QMessageBox::StandardButton reply;
        QString type = openedFileName.isEmpty() ? " as" : "";
        reply = QMessageBox::question(this, "Save file" + type, "Do you want to save file" + type + "?", QMessageBox::Yes | QMessageBox::No);
        if (QMessageBox::Yes == reply)
        {
            if (!openedFileName.isEmpty())
            {
                FileSave();
            }
            else
            {
                FileSaveAs();
            }
        }
    }
}

void MainWindow::FileSave(){

    QString s = QFileDialog::getSaveFileName(this, "Save File", openedFileName, filter);
    if (s.length() > 0)
    {
        QString ext = QString(&(s.data()[s.length() - 4]));
        if (ext == ".txt")
        {
            QFile file(s);
            if (file.open(QFile::WriteOnly))
            {
                QTextStream stream(&file);
                stream << ui->textEdit->toPlainText();
                file.close();
                openedFileName = s;
            }
        }
    }
}

void MainWindow::FileSaveAs(){

    QString s = QFileDialog::getSaveFileName(this, "Save File As", QDir::current().path(), filter);
    if (s.length() > 0)
    {
        QString ext = QString(&(s.data()[s.length() - 4]));
        if (ext == ".txt")
        {
            QFile file(s);
            if (file.open(QFile::WriteOnly))
            {
                QTextStream stream(&file);
                stream << ui->textEdit->toPlainText();
                file.close();
                openedFileName = s;
            }
        }
    }
}

void MainWindow::FileOpen(){

    QString s = QFileDialog::getOpenFileName(this, "Open File", QDir::current().path(), filter);
    if (s.length() > 0)
    {
        int index = s.indexOf(".txt");
        QFile file(s);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            if (index != -1 && s.length() - 4 == index)
            {
                QTextStream stream(&file);
                ui->textEdit->setText(stream.readAll());
                openedFileName = s;
                file.close();
            }
        }
    }
}

void MainWindow::on_actionEnglish_triggered()
{
    changeTranslator("en");
}

void MainWindow::on_actionRussian_triggered()
{
    changeTranslator("ru");
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    if (this->aDirList->at(index.row()).isDir())
    {
        QString tmp = this->aDirList->at(index.row()).absoluteFilePath();
        model->getFolderList(this->aDirList->at(index.row()).absoluteFilePath(),this->aDirList);
        this->ui->lineEdit->setText(tmp);
    } else {
        QDesktopServices::openUrl(QUrl(this->aDirList->at(index.row()).absoluteFilePath()));
    }
}

void MainWindow::on_actionNew_triggered()
{
    ExtendedFileSave();
    ui->textEdit->setReadOnly(false);
    ui->textEdit->setDisabled(false);
    ui->textEdit->clear();
    openedFileName = "";
}


void MainWindow::on_actionOpen_triggered()
{
    ExtendedFileSave();
    FileOpen();

    ui->textEdit->setReadOnly(false);
    ui->textEdit->setDisabled(false);
}



void MainWindow::on_actionClose_triggered()
{
    ExtendedFileSave();
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setDisabled(true);
    ui->textEdit->clear();
    openedFileName = "";
}


void MainWindow::on_actionSave_triggered()
{
    FileSave();
}


void MainWindow::on_actionSaveAs_triggered()
{
    FileSaveAs();
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog* pPrintDialog = new QPrintDialog(&printer);
    if(pPrintDialog->exec() == QDialog::Accepted) {

    }
    delete pPrintDialog;
}


void MainWindow::on_actionLight_triggered()
{
    qApp->setPalette(style()->standardPalette());
}

void MainWindow::on_actionDark_triggered()
{
    QPalette darkPalette;

        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        qApp->setPalette(darkPalette);
}

void MainWindow::on_actionHelp_triggered()
{
    HelpForm* help = new HelpForm(this);
    if(help){

        help->show();
    }
}

void MainWindow::on_actionReadOnly_triggered()
{
    QString s = QFileDialog::getOpenFileName(this, "Open File", QDir::current().path(), filter);
    QFile file(":/.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "????????????????!", "?????????? ???????????? ?????? ????????????! ?? ???????????? - ???????? ???????????? ????????????????!");
    }
    if (s.length() > 0)
    {
        int index = s.indexOf(".txt");
        QFile file(s);
        if (file.open(QFile::ReadOnly))
        {
            if (index != -1 && s.length() - 4 == index)
            {
                QTextStream stream(&file);
                ui->textEdit->setText(stream.readAll());
                openedFileName = s;
                file.close();
            }
        }
    }
}


void MainWindow::on_ActionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::randomSizeOfFont()
{
    QTextCharFormat fmt;
    fmt.setForeground(QBrush(QColor(rand() % 256,rand() % 256, rand() % 256)));
    fmt.setBackground(QBrush(QColor(rand() % 256,rand() % 256, rand() % 256)));
    tEdit->textCursor().setCharFormat(fmt);
}

void MainWindow::setFont()
{
    QFont font = tEdit->textCursor().charFormat().font();

    QFontDialog fntDlg(font,this);

    bool b[] = {true};
    font = fntDlg.getFont(b);

    if (b[0]) {
    QTextCharFormat fmt;
    fmt.setFont(font);
    tEdit->textCursor().setCharFormat(fmt);
    }
}

void MainWindow::convertToHTML()
{
    QString str = tEdit->document()->toHtml();
    QString endTxt = "";
    QChar *chstr = str.data();
    for (int i = 0; i < str.length(); i)
    {
     int index = str.indexOf("</", i);
        if (index != -1)
            {
                index = str.indexOf(">", index);
                    if (index != -1)
                        {
                            index++;
                            endTxt.insert(endTxt.length(), &chstr[i], index - i);
                            endTxt += "\n";
                            i = index;
                        }
                        else
                            {
                                endTxt.insert(endTxt.length(), &chstr[i], str.length() - i);
                                break;
                            }
             }
                else
                    {
                        endTxt.insert(endTxt.length(), &chstr[i], str.length() - i);
                        break;
                    }
      }
        tEdit->document()->setPlainText(endTxt);

}

void MainWindow::convertFromHTML()
{
    QString str = tEdit->document()->toPlainText();
    tEdit->document()->setHtml(str);
}


