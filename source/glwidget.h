#ifndef GLWIDGET_H
#define GLWIDGET_H
#define PI 3.1415926535897932384626433832795

#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "matrix.h"
#include "OMFImport.h"

struct sprite
{
    QOpenGLBuffer vbo;
    QOpenGLBuffer pos_vbo;
    QOpenGLBuffer mag_vbo;
    QOpenGLVertexArrayObject *vao;
    GLuint count;
};

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget( const QGLFormat& format, QWidget* parent = 0 );

    // Data and Drawing
    void updateData(QSharedPointer<OMFReader> data);
//    void updateHeader(QSharedPointer<OMFHeader> header, QSharedPointer<matrix> data);
    void isDoneRendering();
    virtual void renderFrame(QString file);

    // View Preferences
    virtual void toggleDisplay(int type);
    virtual void setBackgroundColor(QColor color);
    virtual void setSpriteDimensions(int newslices, float length, float radius, float tipLengthRatio, float shaftRadiusRatio, QString origin);
    virtual void setBrightness(float bright);
    void setColorScale(QString value);
    void setSpriteScale(QString value);
    void setColoredQuantity(QString value);
    void setCustomColorScale(QList<QColor> colors);

public slots:
    virtual void update();

    // Movement and slicing
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setXSliceLow(int low);
    void setYSliceLow(int low);
    void setZSliceLow(int low);
    void setThresholdLow(int low);
    void setXSliceHigh(int high);
    void setYSliceHigh(int high);
    void setZSliceHigh(int high);
    void setThresholdHigh(int high);
    void setXCom(float val);
    void setYCom(float val);
    void setZCom(float val);
    void setXLoc(float val);
    void setYLoc(float val);
    void setZLoc(float val);

    // Subsampling
    void increaseSubsampling();
    void decreaseSubsampling();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void COMChanged(float val);
    void doneRenderingFrame(QString filename);

protected:
    virtual void updateCOM();
    virtual void updateExtent();

    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();
    virtual void pushBuffers();
    virtual void pushLUT();

    virtual void keyPressEvent( QKeyEvent* e );
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

private:
    // Shaders
    QOpenGLShaderProgram standardShader, cubeShader;
    QOpenGLShaderProgram *currentShader;
    QOpenGLFunctions_3_3_Core* gl330Funcs;

    // Init functions
    void initializeAssets();
    bool initializeShaders();
    bool initializeLights();
    bool initializeCube();
    bool initializeCone(int slices, float radius, float height);
    bool initializeVect(int slices, float height, float radius, float fractionTip, float fractionInner);

    QVector<QVector4D> instPositions;
    QVector<QVector4D> instMagnetizations;

    // Sprites and Data
    sprite cube, cone, vect;
    sprite *displayObject;
    int numNodes; // Number of nodes being displayed with current subsampling
    int displayType; // Cube 0, Cone 1, Vector 2
    int valuedim;    // scalar or vector
    int subsampling; // display each 2^n'th cell according to this variable
    QSharedPointer<OMFReader> dataPtr;
    float maxmag, minmag;
    QColor spriteColor;
    int slices; // Resolution for cones/vects
    float vectorLength, vectorRadius, vectorTipLengthRatio, vectorShaftRadiusRatio;

    // Lighting
    GLfloat   lightAmbient;
    QVector4D lightIntensity;
    QVector4D lightPosition;
    float brightness;

    // View Related
    QSize mandatedSize;
    QColor backgroundColor;
    QMatrix4x4 projection, view, model;
    bool displayOn;
    QVector3D com, location, maxExtent, minExtent;
    float xcom, ycom, zcom;
    float xmax, ymax, zmax;
    float xmin, ymin, zmin;
    int xRot, yRot, zRot;
    float xLoc, yLoc, zLoc, zoom;
    // slice variables
    int xSliceLow, xSliceHigh;
    int ySliceLow, ySliceHigh;
    int zSliceLow, zSliceHigh;
    int thresholdLow, thresholdHigh;

    // Coloring
    QString colorScale;
    QString coloredQuantity;
    QString vectorOrigin;
    QString spriteScale;
    QColor customSpriteColor(float val); // val goes from 0 to 1.0
    QList<QColor> customColors;
    QMap<QString, int> display_type_map;

    // Mouse control
    QVector2D previousMousePosition;
    bool leftMousePressed;
    bool middleMousePressed;
    bool rightMousePressed;

    // Render control
    bool needsUpdate;
    QString filename; // for rendering image sequences...

};

#endif // GLWIDGET_H

