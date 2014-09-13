#include "customtabwidget.h"

void customTabWidget::moveTab(int fromIndex, int toIndex)
{

    if (fromIndex!=0)
    {
        int to=0;
        if(toIndex==0)
        {
            to=1;
        }
        QWidget* w = widget(fromIndex);
        QIcon icon = tabIcon(fromIndex);
        QString text = tabText(fromIndex);

        removeTab(fromIndex);
        insertTab(to, w, icon, text);
        setCurrentIndex(toIndex);
    }
}
