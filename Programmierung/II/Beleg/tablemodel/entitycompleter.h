/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
EntityCompleter is a QCompleter that can be used to autocomplete entities in a QAbstractItemModel.
It is used to autocomplete the entities in the addpanel and infopanel.
*/

#ifndef _ENTITYCOMPLETER_H
#define _ENTITYCOMPLETER_H

#include <QCompleter>
#include <QAbstractItemModel>
#include <QStringList>
#include <QString>
#include <QStringListModel>
#include <QRegularExpression>

class EntityCompleter : public QCompleter
{
    Q_OBJECT

    private:
        QAbstractItemModel* sourceModel; // the model to get the completions from
        QStringList filterColumns; // the columns to filter the completions by
        QString returnFormat; // the format to return the completions in e.g. ({id})
        QString displayFormat; // the format to display the completions in e.g. {firstname} {lastname} ({id})
        QString searchDelimiter; // the delimiter to split the search string into a list of strings

        QStringListModel* completionModel;
        QStringList allCompletions;

        void buildCompletionModel();
        int getColumnIndex(const QString& columnName) const;
        QString buildDisplayString(int row) const;
        bool rowMatchesTokens(int row, const QStringList& tokens) const;
        
    public:
        EntityCompleter(QAbstractItemModel* sourceModel, QString returnFormat, QStringList filterColumns, QString displayFormat, QString searchDelimiter, QObject* parent = nullptr);
        ~EntityCompleter();

        // setters
        void setSourceModel(QAbstractItemModel* sourceModel);
        void setFilterColumns(const QStringList& filterColumns);
        void setReturnFormat(const QString& returnFormat);
        void setDisplayFormat(const QString& displayFormat);
        void setSearchDelimiter(const QString& searchDelimiter);

        // getters
        QAbstractItemModel* getSourceModel() const;
        QStringList getFilterColumns() const;
        QString getReturnFormat() const;
        QString getDisplayFormat() const;
        QString getSearchDelimiter() const;

        QStringList splitPath(const QString& path) const override;
        QString pathFromIndex(const QModelIndex& index) const override;

        void updateCompletions(const QString &prefix);
        void setSearchPrefix(const QString &prefix);

    private slots:
        void onActivated(const QString &completion);
};

#endif