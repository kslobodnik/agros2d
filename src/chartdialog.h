#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QPrintDialog>
#include <QtGui/QListWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>
#include <QFileDialog>
#include <QColor>
#include <QAction>
#include <QPrinter>

#include <qwt_scale_map.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_printfilter.h>
#include <qwt_counter.h>
#include <qwt_legend.h>
#include <qwt_text.h>
#include <qwt_plot.h>

#include <math.h>

#include "scene.h"
#include "util.h"
#include "localvalueview.h"

class Chart;

class ChartDialog : public QDialog
{
    Q_OBJECT

signals:
    void setChartLine(const Point &start, const Point &end);

public slots:
    void doPlot();

public:
    ChartDialog(QWidget *parent = 0);
    ~ChartDialog();

    void showDialog();

protected:
    void hideEvent(QHideEvent *event);

private:
    QTabWidget* tabWidget;

    QwtPlotPicker *picker;

    QLabel *lblStartX;
    QLabel *lblStartY;
    QLabel *lblEndX;
    QLabel *lblEndY;

    SLineEdit *txtStartX;
    SLineEdit *txtStartY;
    SLineEdit *txtEndX;
    SLineEdit *txtEndY;

    QRadioButton *radAxisLength;
    QRadioButton *radAxisX;
    QRadioButton *radAxisY;
    QSpinBox *txtAxisPoints;

    QComboBox *cmbFieldVariable;
    QComboBox *cmbFieldVariableComp;

    Chart *chart;
    QTableWidget *trvTable;

    void createControls();

private slots:
    void doFieldVariable(int index);
    void doPrint();
    void doSaveImage();
    void doExportData();
    void doMoved(const QPoint &);
    void doChartLine();
};

#endif // CHARTDIALOG_H
