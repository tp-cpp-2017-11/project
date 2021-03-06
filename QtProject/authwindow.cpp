#include "authwindow.h"
#include "ui_authwindow.h"

AuthWindow::AuthWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    authResult = false;
    attempts = 3;

    connect(ui->buttonLogin, SIGNAL(clicked(bool)), this, SLOT(authentication()));
    connect(ui->buttonCreateMaster, SIGNAL(clicked(bool)), this, SLOT(createMaster()));
    connect(ui->editNewMaster, SIGNAL(textChanged(QString)), this, SLOT(passwordStrengthWatcher()));

    connect(ui->eyeMaster, SIGNAL(clicked(bool)), this, SLOT(changeMasterVisibility()));
    connect(ui->eyeNewMaster, SIGNAL(clicked(bool)), this, SLOT(changeNewMasterVisibility()));
    connect(ui->eyeConfirmMaster, SIGNAL(clicked(bool)), this, SLOT(changeMasterConfirmVisibility()));

    Data::Instance().GetMaster();
    std::string masterHash = Data::Instance().masterHash;
    if(masterHash.empty())
    {
        ui->groupLogin->hide();
        ui->buttonCreateMaster->setDefault(true);
    }
    else
    {
        ui->groupCreateMaster->hide();
        ui->buttonLogin->setDefault(true);
    }
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::authentication()
{
    std::string password = ui->editMaster->text().toStdString();
    std::string passwordHash = Encryptor::Instance().Hash(password);

    if(passwordHash == Data::Instance().masterHash)
    {
        authResult = true;
        attempts = 3;
        Encryptor::Instance().DerieveKey(password);
        Data::Instance().GetData();
        close();
    }
    else
    {
        attempts--;
        if(attempts == 0)
        {
            QMessageBox::critical(nullptr,"Error!","\nLogon limit exceeded!\n",QMessageBox::Ok);
            close();
        }
        QMessageBox::critical(nullptr,"Error!","\nAuthentication error!\n",QMessageBox::Ok);
    }
}

void AuthWindow::createMaster()
{
    QString password = ui->editNewMaster->text();
    QString confirmPassword = ui->editConfirmMaster->text();

    if(password != confirmPassword)
    {
        QMessageBox::warning(nullptr,"Error!", "\nPasswords doesn't match!\n", QMessageBox::Ok);
    }
    else
    {
        Encryptor::Instance().DerieveKey(password.toStdString());
        std::string hash = Encryptor::Instance().Hash(password.toStdString());
        Data::Instance().GetData();
        Data::Instance().masterHash = hash;
        Data::Instance().Save();
        authResult = true;
        close();
    }

}

void AuthWindow::passwordStrengthWatcher()
{
    std::string password = ui->editNewMaster->text().toStdString();
    PasswordStrength strength =PasswordStrengthChecker::Instance().CheckPasswordStrength(password);
    QPalette pallete = ui->labelPasswordStrengthLevel->palette();
    switch (strength) {
        case NO_PASSWORD:
            ui->labelPasswordStrengthLevel->clear();
            break;
        case WEAK:
            ui->labelPasswordStrengthLevel->setText("Weak");
            pallete.setColor(ui->labelPasswordStrengthLevel->foregroundRole(), Qt::red);
            ui->labelPasswordStrengthLevel->setPalette(pallete);
            break;
        case MEDIUM:
            ui->labelPasswordStrengthLevel->setText("Medium");
            pallete.setColor(ui->labelPasswordStrengthLevel->foregroundRole(), QColor(251, 192, 45));
            ui->labelPasswordStrengthLevel->setPalette(pallete);
            break;
        case STRONG:
            ui->labelPasswordStrengthLevel->setText("Strong");
            pallete.setColor(ui->labelPasswordStrengthLevel->foregroundRole(), QColor(102, 187, 106));
            ui->labelPasswordStrengthLevel->setPalette(pallete);
            break;
        case BEST:
            ui->labelPasswordStrengthLevel->setText("Best");
            pallete.setColor(ui->labelPasswordStrengthLevel->foregroundRole(), Qt::darkGreen);
            ui->labelPasswordStrengthLevel->setPalette(pallete);
            break;
        default:
            break;
    }
}

void AuthWindow::changeMasterVisibility()
{
    if(ui->eyeMaster->isChecked())
        FieldManager::Instance().showField(ui->editMaster);
    else
        FieldManager::Instance().hideField(ui->editMaster);
}

void AuthWindow::changeNewMasterVisibility()
{
    if(ui->eyeNewMaster->isChecked())
        FieldManager::Instance().showField(ui->editNewMaster);
    else
        FieldManager::Instance().hideField(ui->editNewMaster);
}

void AuthWindow::changeMasterConfirmVisibility()
{
    if(ui->eyeConfirmMaster->isChecked())
        FieldManager::Instance().showField(ui->editConfirmMaster);
    else
        FieldManager::Instance().hideField(ui->editConfirmMaster);
}
