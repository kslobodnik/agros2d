#ifndef SCENE_H
#define SCENE_H

#include <QtGui/QMessageBox>
#include <QDesktopServices>
#include <QObject>
#include <QList>
#include <QDate>
#include <QDomDocument>
#include <QTextStream>
#include <QLocale>
#include <QFile>
#include <QAction>

#include <dl_dxf.h>
#include <dl_creationadapter.h>

#include "util.h"
#include "locale.h"
#include "scenemarker.h"
#include "scenebasic.h"
#include "projectdialog.h"
#include "scenetransformdialog.h"

#include "solverdialog.h"
#include "helpdialog.h"
#include "scenesolution.h"
#include "hermes2d/hermes_electrostatic.h"
#include "hermes2d/hermes_magnetostatic.h"
#include "hermes2d/hermes_harmonicmagnetic.h"
#include "hermes2d/hermes_heat.h"
#include "hermes2d/hermes_current.h"
#include "hermes2d/hermes_elasticity.h"

class ThreadSolver;

class SceneNode;
class SceneEdge;
class SceneLabel;

class SceneEdgeMarker;
class SceneLabelMarker;

struct SceneViewSettings;
class SceneSolution;

class SolverDialog;
class ProjectDialog;
class SceneTransformDialog;

struct ProjectInfo
{
    QString name;
    QDate date;
    QString fileName;
    ProblemType problemType;
    PhysicField physicField;
    int numberOfRefinements;
    int polynomialOrder;
    int adaptivitySteps;
    double adaptivityTolerance;
    QString scriptStartup;
    int frequency;

    ProjectInfo()
    {
        clear();
    }

    void clear()
    {
        name = QObject::tr("unnamed");
        date = QDate::currentDate();
        fileName = "";
        scriptStartup = "";
        // physicField = PHYSICFIELD_ELECTROSTATIC;
        physicField = PHYSICFIELD_HARMONIC_MAGNETIC;
        problemType = PROBLEMTYPE_PLANAR;

        numberOfRefinements = 1;
        polynomialOrder = 3;
        adaptivitySteps = 0;
        adaptivityTolerance = 1.0;

        frequency = 0.0;
    }

    inline QString labelX() { return ((problemType == PROBLEMTYPE_PLANAR) ? "X" : "R");  }
    inline QString labelY() { return ((problemType == PROBLEMTYPE_PLANAR) ? "Y" : "Z");  }
};

class DxfFilter : public DL_CreationAdapter
{
public:
    DxfFilter(Scene *scene);

    void addArc(const DL_ArcData& a);
    void addLine(const DL_LineData& d);
private:
    Scene *m_scene;
};

class Scene : public QObject {
    Q_OBJECT

public slots:
    void doNewNode(const Point &point = Point());
    void doNewEdge();
    void doNewLabel();
    void doNewEdgeMarker();
    void doNewLabelMarker();  
    void doTransform();
    void doProjectProperties();

signals:
    void invalidated();
    void solved();

public:
    QList<SceneNode *> nodes;
    QList<SceneEdge *> edges;
    QList<SceneLabel *> labels;

    QList<SceneEdgeMarker *> edgeMarkers;
    QList<SceneLabelMarker *> labelMarkers;

    QAction *actNewNode;
    QAction *actNewEdge;
    QAction *actNewLabel;
    QAction *actNewEdgeMarker;
    QAction *actNewLabelMarker;
    QAction *actProjectProperties;
    QAction *actTransform;

    Scene();
    ~Scene();

    SceneNode *addNode(SceneNode *node);
    void removeNode(SceneNode *node);

    SceneEdge *addEdge(SceneEdge *edge);
    void removeEdge(SceneEdge *edge);

    SceneLabel *addLabel(SceneLabel *label);
    void removeLabel(SceneLabel *label);

    void addEdgeMarker(SceneEdgeMarker *edgeMarker);
    void removeEdgeMarker(SceneEdgeMarker *edgeMarker);
    void setEdgeMarker(SceneEdgeMarker *edgeMarker); // set edge marker to selected edges

    void addLabelMarker(SceneLabelMarker *labelMarker);
    void removeLabelMarker(SceneLabelMarker *labelMarker);
    void setLabelMarker(SceneLabelMarker *labelMarker); // set label marker to selected labels

    void clear();

    RectPoint boundingBox();

    void selectNone();
    void selectAll(SceneMode sceneMode);
    void highlightNone();
    void deleteSelected();

    void transformTranslate(const Point &point, bool copy);
    void transformRotate(const Point &point, double angle, bool copy);
    void transformScale(const Point &point, double scaleFactor, bool copy);

    inline ProjectInfo &projectInfo() { return m_projectInfo; }

    inline void refresh() { emit invalidated(); }
    void createMeshAndSolve(SolverMode solverMode);
    inline SceneSolution *sceneSolution() { return m_sceneSolution; }

    int writeToTriangle();
    bool triangle2mesh(const QString &source, const QString &destination);
    void readFromDxf(const QString &fileName);
    void writeToDxf(const QString &fileName);
    void readFromFile(const QString &fileName);
    void writeToFile(const QString &fileName);

protected:

private:    
    ProjectInfo m_projectInfo;

    // scene solution
    SceneSolution *m_sceneSolution;

    // solver dialog
    SolverDialog *solverDialog;

    void createActions();

private slots:    
    void doInvalidated();
    void doSolved();
};

// **************************************************************************************

class Util
{
public:
    static Scene *scene()
    {
        static Scene m_scene;
        return &m_scene;
    }

    static HelpDialog *helpDialog()
    {
        static HelpDialog helpDialog;
        return &helpDialog;
    }
};

#endif /* SCENE_H */