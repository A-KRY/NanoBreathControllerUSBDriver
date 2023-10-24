//
// Created by Admin on 2023/10/23.
//

#include <iostream>
#include "LineEditValidator.h"

#define INFO(TEXT) QMessageBox::information(nullptr, "info", (TEXT), QMessageBox::Ok);

QValidator::State LineEditValidator::validate(QString &string, int &i) const {
    QRegularExpression rx("\\d+");
    auto match = rx.match(string);
    if (match.hasMatch() and match.captured(0) == string) {
        qDebug() << "Acceptable";
        return QValidator::Acceptable;
    }
    else {
        qDebug("Invalid");
        return QValidator::Invalid;
    }
}
