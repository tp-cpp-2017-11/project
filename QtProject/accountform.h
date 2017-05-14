#ifndef ACCOUNTFORM_H
#define ACCOUNTFORM_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include "Account.h"
#include "Data.h"
#include "passwordgenerator.h"

namespace Ui {
class accountform;
}

class accountform : public QDialog
{
    Q_OBJECT

public:
    explicit accountform(QWidget *parent = 0);
    ~accountform();

    void set_account_name(std::string name);

private:
    Ui::accountform *ui;

    std::string account_name;

private slots:

    void save_account();
    void toggleShowPassword();
    void toggleHidePassword();
    void generatePassword();

};

#endif // ACCOUNTFORM_H