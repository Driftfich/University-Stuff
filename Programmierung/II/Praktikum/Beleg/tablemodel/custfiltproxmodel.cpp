/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the CustomFilterProxyModel class, which is a custom filter proxy model building on top of the QSortFilterProxyModel.
Purpose: Custom filter logic for the table view. The filter string is split into tokens by a delimiter.
Each row is accepted if it contains all of the tokens in any columns. This allows flexible multiple column searches.
*/

#include "custfiltproxmodel.h"
#include <QAbstractItemModel>
#include <QRegularExpression>
#include <QList>

// set new search tokens based on the filter string and delimiter
void CustomFilterProxyModel::setSearchString(const QString &filterText, const QString &delimiter) {
    // clear previous search tokens
    searchTokens.clear();

    // split into tokens by delimiter
    QStringList newTokens = filterText.split(delimiter, Qt::SkipEmptyParts);

    // remove duplicate tokens
    newTokens.removeDuplicates();

    for (QString tok : newTokens) {
        // create a case-insensitive regex for each token
        searchTokens.append(QRegularExpression(tok, QRegularExpression::CaseInsensitiveOption));
    }

    invalidateFilter();
}

// override the filterAcceptsRow method to implement the custom filter logic
bool CustomFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    if (searchTokens.isEmpty()) {
        return true; // no filter applied
    }
    // get the source data model
    const QAbstractItemModel *m = sourceModel();
    // if no source model, accept all rows
    if (!m) {
        return true;
    }

    // iterate over the search tokens
    for (const QRegularExpression &token : searchTokens) {
        bool found = false;
        // iterate over the columns
        for (int col=0; col < m->columnCount(); ++col) {
            // get the index of the cell
            QModelIndex idx = m->index(sourceRow, col, sourceParent);
            // if the index is invalid, continue
            if (!idx.isValid()) continue;
            // get the cell data
            QString cell = m->data(idx, Qt::DisplayRole).toString().toLower();
            // if the cell matches the token, this token is found in the row and the next token can be checked
            if (token.match(cell).hasMatch()) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false; // if any token is not found in the row, reject the row
        }
    }
    
    return true; // all tokens found in the row, accept it
}