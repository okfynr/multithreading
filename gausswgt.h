#ifndef GAUSSWGT_H
#define GAUSSWGT_H

#include <QWidget>

class QLineEdit;
class QGridLayout;
class QPushButton;
class QComboBox;
class QSpinBox;

class GaussWgt : public QWidget
{
	Q_OBJECT

	QGridLayout * _lay;
	QLineEdit * _input_00;
	QLineEdit * _input_01;
	QLineEdit * _input_02;

	QLineEdit * _input_10;
	QLineEdit * _input_11;
	QLineEdit * _input_12;

	QLineEdit * _input_20;
	QLineEdit * _input_21;
	QLineEdit * _input_22;

	QComboBox * _choose_type;
	QSpinBox * _matrix_size_box;

	QPushButton * _buttonCompute;

public:
	explicit GaussWgt(QWidget *parent = nullptr);
	~GaussWgt(){}

signals:

private slots:
	void computation();
};

#endif // GAUSSWGT_H
