
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

#include "gausswgt.h"

GaussWgt::GaussWgt(QWidget *parent) : QWidget(parent)
{

	_lay = new QGridLayout(this);

	_buttonCompute = new QPushButton("compute", this);
	_choose_type = new QComboBox(this);
	_choose_type->addItem("solve system", 0);
	_choose_type->addItem("inverse matrix", 1);
	_choose_type->addItem("LU decompose", 2);

	_matrix_size_box = new QSpinBox(this);
	_matrix_size_box->setRange(3, 200);


	_input_00 = new QLineEdit("00", this);
	_input_01 = new QLineEdit("01", this);
	_input_02 = new QLineEdit("02", this);

	_input_10 = new QLineEdit("10", this);
	_input_11 = new QLineEdit("11", this);
	_input_12 = new QLineEdit("12", this);

	_input_20 = new QLineEdit("20", this);
	_input_21 = new QLineEdit("21", this);
	_input_22 = new QLineEdit("22", this);

	_lay->addWidget(_input_00, 0, 0);
	_lay->addWidget(_input_01, 0, 1);
	_lay->addWidget(_input_02, 0, 2);

	_lay->addWidget(_input_10, 1, 0);
	_lay->addWidget(_input_11, 1, 1);
	_lay->addWidget(_input_12, 1, 2);

	_lay->addWidget(_input_20, 2, 0);
	_lay->addWidget(_input_21, 2, 1);
	_lay->addWidget(_input_22, 2, 2);

	_lay->addWidget(new QLabel("matrix size: "), 3, 1);
	_lay->addWidget(_matrix_size_box, 3, 2);
	_lay->addWidget(_choose_type, 4, 1);
	_lay->addWidget(_buttonCompute, 4, 2);

	setLayout(_lay);

	connect(_buttonCompute, SIGNAL(clicked()), this, SLOT(computation()));
}

void GaussWgt::computation()
{
	switch(_choose_type->currentData().toInt())
	{
	case 0:
		qDebug() << "solve system";
		break;
	case 1:
		qDebug() << "inverse matrix";
		break;
	case 2:
		qDebug() << "LU decompose";
		break;
	default:
		return;
	}

}
