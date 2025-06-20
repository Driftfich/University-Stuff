/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
EntityCompleter is a QCompleter that can be used to autocomplete entities in a QAbstractItemModel.
*/

#include <QCompleter>
#include <QAbstractItemModel>
#include <QStringList>
#include <QString>
#include <QStringListModel>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

#include "entitycompleter.h"

EntityCompleter::EntityCompleter(QAbstractItemModel* sourceModel, QString returnFormat, QStringList filterColumns, QString displayFormat, QString searchDelimiter, QObject* parent)
    : QCompleter(parent), sourceModel(sourceModel), returnFormat(returnFormat), filterColumns(filterColumns), displayFormat(displayFormat), searchDelimiter(searchDelimiter) {
    completionModel = new QStringListModel(this);
    setModel(completionModel); // Set the completion model on QCompleter
    buildCompletionModel();
    
    // Connect activation signal
    connect(this, QOverload<const QString &>::of(&QCompleter::activated),
            this, &EntityCompleter::onActivated);
}

EntityCompleter::~EntityCompleter() {
    // No need to delete completionModel - QCompleter takes ownership
}

// Setters
void EntityCompleter::setSourceModel(QAbstractItemModel* sourceModel) {
    this->sourceModel = sourceModel;
    buildCompletionModel();
}

void EntityCompleter::setFilterColumns(const QStringList& filterColumns) {
    this->filterColumns = filterColumns;
    buildCompletionModel();
}

void EntityCompleter::setReturnFormat(const QString& returnFormat) {
    this->returnFormat = returnFormat;
}

void EntityCompleter::setDisplayFormat(const QString& displayFormat) {
    this->displayFormat = displayFormat;
    buildCompletionModel();
}

void EntityCompleter::setSearchDelimiter(const QString& searchDelimiter) {
    this->searchDelimiter = searchDelimiter;
}

// Getters
QAbstractItemModel* EntityCompleter::getSourceModel() const {
    return sourceModel;
}

QStringList EntityCompleter::getFilterColumns() const {
    return filterColumns;
}

QString EntityCompleter::getReturnFormat() const {
    return returnFormat;
}

QString EntityCompleter::getDisplayFormat() const {
    return displayFormat;
}

QString EntityCompleter::getSearchDelimiter() const {
    return searchDelimiter;
}

// Helper method to get column index by name
int EntityCompleter::getColumnIndex(const QString& columnName) const {
    if (!sourceModel) return -1;
    
    for (int i = 0; i < sourceModel->columnCount(); i++) {
        QString headerData = sourceModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        if (headerData == columnName) {
            return i;
        }
    }
    return -1;
}

// Build completion string for display (using displayFormat)
QString EntityCompleter::buildDisplayString(int row) const {
    QRegularExpression regex("\\{([^}]+)\\}");
    QStringList keys;
    
    QRegularExpressionMatchIterator iterator = regex.globalMatch(displayFormat);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        keys << match.captured(1);
    }

    QString result = displayFormat;
    for (const QString& key : keys) {
        int columnIndex = getColumnIndex(key);
        if (columnIndex >= 0) {
            QModelIndex dataIndex = sourceModel->index(row, columnIndex);
            QString value = sourceModel->data(dataIndex, Qt::DisplayRole).toString();
            result.replace("{" + key + "}", value);
        }
    }
    return result;
}

// Check if a row matches all search tokens
bool EntityCompleter::rowMatchesTokens(int row, const QStringList& tokens) const {
    if (tokens.isEmpty()) return true;
    
    // All tokens must match (AND logic)
    for (const QString& token : tokens) {
        if (token.trimmed().isEmpty()) continue;
        
        bool tokenFound = false;
        // Search through filter columns only
        for (const QString& columnName : filterColumns) {
            int columnIndex = getColumnIndex(columnName);
            if (columnIndex >= 0) {
                QModelIndex dataIndex = sourceModel->index(row, columnIndex);
                QString cellData = sourceModel->data(dataIndex, Qt::DisplayRole).toString();
                if (cellData.contains(token, Qt::CaseInsensitive)) {
                    tokenFound = true;
                    break;
                }
            }
        }
        
        // If any token doesn't match, this row is excluded
        if (!tokenFound) {
            return false;
        }
    }
    return true;
}

// Split the path into a list of strings
QStringList EntityCompleter::splitPath(const QString& path) const {
    if (path.contains(searchDelimiter)) {
        return path.split(searchDelimiter, Qt::SkipEmptyParts);
    }
    return QStringList() << path;
}

// Build a string with the given return format (what gets inserted)
QString EntityCompleter::pathFromIndex(const QModelIndex& index) const {
    QRegularExpression regex("\\{([^}]+)\\}");
    QStringList keys;
    
    QRegularExpressionMatchIterator iterator = regex.globalMatch(returnFormat);
    while (iterator.hasNext()) {
        QRegularExpressionMatch match = iterator.next();
        keys << match.captured(1);
    }

    QString result = returnFormat;
    for (const QString& key : keys) {
        int columnIndex = getColumnIndex(key);
        if (columnIndex >= 0) {
            // Get the actual row from the source model
            int sourceRow = index.row();
            QModelIndex dataIndex = sourceModel->index(sourceRow, columnIndex);
            QString value = sourceModel->data(dataIndex, Qt::DisplayRole).toString();
            result.replace("{" + key + "}", value);
        }
    }
    return result;
}

void EntityCompleter::buildCompletionModel() {
    if (!sourceModel) return;
    
    allCompletions.clear();
    for (int i = 0; i < sourceModel->rowCount(); i++) {
        QString displayString = buildDisplayString(i);
        allCompletions.append(displayString);
    }
    completionModel->setStringList(allCompletions);
}

void EntityCompleter::updateCompletions(const QString &prefix) {
    if (!sourceModel) return;
    
    QStringList tokens = splitPath(prefix);
    QStringList filteredCompletions;
    
    for (int i = 0; i < sourceModel->rowCount(); i++) {
        if (rowMatchesTokens(i, tokens)) {
            QString displayString = buildDisplayString(i);
            filteredCompletions.append(displayString);
        }
    }
    
    completionModel->setStringList(filteredCompletions);
}

void EntityCompleter::setSearchPrefix(const QString &prefix) {
    updateCompletions(prefix);
    setCompletionPrefix(prefix);
}

void EntityCompleter::onActivated(const QString &completion) {
    // Handle completion activation if needed
    // This is where you could emit custom signals or perform additional logic
}
