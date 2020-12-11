#pragma once
#include <QMouseEvent>
#include <QPushButton>
#include <QString>

class RightClickableButton : public QPushButton {
    Q_OBJECT
public:
    explicit RightClickableButton(QWidget* parent = 0);
    explicit RightClickableButton(unsigned int parent_id, const QString& text, QWidget* parent = 0);
    ~RightClickableButton();

private slots:
    void mousePressEvent(QMouseEvent* event);

signals:
    void rightClicked();
    void clicked(unsigned int id);

private:
    unsigned int m_parent_index;
};