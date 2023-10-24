//
// Created by Admin on 2023/10/23.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_LINEEDITVALIDATOR_H
#define NANOBREATHCONTROLLERUSBDRIVER_LINEEDITVALIDATOR_H

#include <QValidator>
#include <QMessageBox>
#include <QDebug>

class LineEditValidator : public QValidator {
public:
    State validate(QString &string, int &i) const override;
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_LINEEDITVALIDATOR_H
