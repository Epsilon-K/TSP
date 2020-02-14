#ifndef UTILS_H
#define UTILS_H
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <stdlib.h>
#include <QTimeLine>
#include <QCompleter>
#include <qmath.h>
#include <QTimer>
#include <math.h>

/* -----------------------------------------------------------------
    General static functions I use everywhere
*/ // --------------------------------------------------------------

class Utils
{
public:
    Utils();

    static QString getNameOfPath(QString path){
        return path.right(path.size() - path.lastIndexOf("/") - 1 );
    }

    static void clamp(float * v, float min, float max){
        if(*v > max)  *v = max;
        else if(*v < min) *v = min;
    }

    static void clamp(int * v, int min, int max){
        if(*v > max)  *v = max;
        else if(*v < min) *v = min;
    }

    static void clamp(double * v, double min, double max){
        if(*v > max)  *v = max;
        else if(*v < min) *v = min;
    }

    static QString getNameWOExtension(QString s){
        return s.left(s.size() - 4);
    }

    static void showMSG(QString title, QString text){
        QMessageBox * msg = new QMessageBox();
        msg->setModal(false);
        msg->setWindowTitle(title);
        msg->setText(text);
        msg->exec();
    }

    static QString getRandomString(int size){
        QString str;
        for(int i = 1; i <= size; i++){
            char c = QString::number(rand()%10)[0].toLatin1();
            str += c;
        }
        return str;
    }

    static QString getTillMark(QString *str, QString mark){
        int ind = str->indexOf(mark);
        QString res = str->left(ind);
        return res;
    }

    static QString getThenDeleteTillMark(QString *str, QString mark){
        int ind = str->indexOf(mark);
        QString res = str->left(ind);
        QString *left = new QString(str->right(str->length() - res.length() - mark.size()));
        *str = *left;
        return res;
    }

    static QString getContentOfTag(QString tagName, QString *str){
        int stInd = str->indexOf("<"+tagName);

        if(stInd == -1){
            return "Not Found";
        }

        stInd += tagName.size();
        str->remove(0,stInd+2);
        int endInd = str->indexOf(tagName+"/>");
        QString content = str->left(endInd-1);
        str->remove(0,endInd + tagName.size()+2);

        return content;
    }



    // Parses a string like "{3, 4, 5,  87, 110}"
    // to a QStringList of those elements
    static QStringList getValuesList(QString valStr){
        QStringList ls = valStr.split(",", QString::SkipEmptyParts);

        QString s = ls[0];
        s.remove("{");
        ls[0] = s;

        s = ls[ls.size()-1];
        s.remove("}");
        ls[ls.size()-1] = s;


        // remove spaces
        for(int i = 0; i < ls.size(); i++){
            s = ls[i];
            bool inQ = false;
            for(int k = 0; k < s.size(); k++){
                if(s[k] == '"'){
                    if(k == 0) inQ = true;
                    else{
                        if(s[k-1] != '\\') inQ = !inQ;
                    }
                }

                if(s[k] == ' '){
                    if(!inQ) s.remove(k,1);
                }
            }
            ls[i] = s;
        }

        return ls;
    }

    static QStringList deleteWhiteSpace(QStringList ls){
        QString s;
        for(int i = 0; i < ls.size(); i++){
            s = ls[i];
            for(int j = 0; j < s.size(); j++){
                if(s.at(j).isSpace()){
                     s.remove(j,1);
                     j--;
                }
            }
            ls[i] = s;
        }
        return ls;
    }

    static QString deleteWhiteSpace(QString s){
        for(int j = 0; j < s.size(); j++){
            if(s.at(j).isSpace()){
                 s.remove(j,1);
                 j--;
            }
        }
        return s;
    }

    static QString colorToString(QColor c){
        QString str = "rgb(" + QString::number(c.red());
        str += ", " + QString::number(c.green()) +
                ", " + QString::number(c.blue()) + ");";
        return str;
    }
};

#endif // UTILS_H
