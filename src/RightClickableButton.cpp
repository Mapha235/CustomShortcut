#include "RightClickableButton.hpp"

RightClickableButton::RightClickableButton(QWidget* parent)
    : QPushButton(parent)

{
}

RightClickableButton::RightClickableButton(unsigned int parent_index, const QString& text, QWidget* parent)
    : QPushButton(text, parent)
    , m_parent_index(parent_index)
{
}

RightClickableButton::~RightClickableButton()
{
}

void RightClickableButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        emit rightClicked();
    else if (event->button() == Qt::LeftButton) {
        emit clicked(m_parent_index);
    }
}

#include "moc_RightClickableButton.cpp"