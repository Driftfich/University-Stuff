#include "custfiltproxmodel.h"
#include <QAbstractItemModel>
#include <QRegularExpression>
#include <QList>

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

bool CustomFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    if (searchTokens.isEmpty()) {
        return true; // no filter applied
    }

    const QAbstractItemModel *m = sourceModel();
    if (!m) {
        return true; // no source model, accept all rows
    }

    for (const QRegularExpression &token : searchTokens) {
        bool found = false;
        for (int col=0; col < m->columnCount(); ++col) {
            QModelIndex idx = m->index(sourceRow, col, sourceParent);
            if (!idx.isValid()) continue;
            QString cell = m->data(idx, Qt::DisplayRole).toString().toLower();
            if (token.match(cell).hasMatch()) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false; // if any token is not found in the row, reject it
        }
    }
    
    return true; // all tokens found in the row, accept it
}