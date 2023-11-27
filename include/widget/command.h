#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QPair>
#include <QCompleter>
#include <QStringListModel>
#include <QFileDialog>
#include <QVector>
#include <QDebug>

#define GROUP_NUM 7
#define GROUP_DATA_NUM 4

enum UserDataSequence
{
    USER_NAME,
    USER_NUMBER,
    USER_DESCRIBE,
    USER_LAST_LOGIN_TIME,
    USER_PROFILE_PIC,
    USER_ROLE,
};

enum GroupDataSequence
{
    GROUP_NAME,
    GROUP_LAST_MODIFIER,
    GROUP_LAST_UPDATE_TIME,
    GROUP_BACKGROUND_PIC,
};
