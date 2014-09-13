#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H
#include<QTabWidget>

class customTabWidget:QTabWidget
{

public slots:
       void moveTab(int fromIndex, int toIndex);

};

#endif // CUSTOMTABWIDGET_H
