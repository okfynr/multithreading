#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <random>
#include <chrono>


#include "gausswgt.h"
#include "mainwindow.h"
#include "multi_funcs.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	_tabs = new QTabWidget(this);

	QWidget * widget_muls = new QWidget();
	_grid = new QGridLayout(widget_muls);

	_buttonCompute = new QPushButton("compute", this);

	_input1_11 = new QLineEdit("00", this);
	_input1_12 = new QLineEdit("01", this);
	_input1_21 = new QLineEdit("10", this);
	_input1_22 = new QLineEdit("11", this);

	_input2_11 = new QLineEdit("00", this);
	_input2_12 = new QLineEdit("01", this);
	_input2_21 = new QLineEdit("10", this);
	_input2_22 = new QLineEdit("11", this);

	_mat_size = new QSpinBox(this);
	_mat_size->setRange(2, 200);
	_output_11 = new QLabel("00", this);
	_output_12 = new QLabel("01", this);
	_output_21 = new QLabel("10", this);
	_output_22 = new QLabel("11", this);


	_grid->addWidget(_input1_11, 0, 0);
	_grid->addWidget(_input1_12, 0, 1);
	_grid->addWidget(_input1_21, 1, 0);
	_grid->addWidget(_input1_22, 1, 1);

	_grid->addWidget(_input2_11, 0, 2);
	_grid->addWidget(_input2_12, 0, 3);
	_grid->addWidget(_input2_21, 1, 2);
	_grid->addWidget(_input2_22, 1, 3);


	_grid->addWidget(new QLabel("matrix size:"), 3, 2);
	_grid->addWidget(_mat_size, 3, 3);

	_grid->addWidget(_output_11, 3, 0);
	_grid->addWidget(_output_12, 3, 1);
	_grid->addWidget(_output_21, 4, 0);
	_grid->addWidget(_output_22, 4, 1);

	_grid->addWidget(_buttonCompute, 4, 3);

	widget_muls->setLayout(_grid);
	_tabs->addTab(widget_muls, "multiplies");

	GaussWgt * widget_gauss = new GaussWgt();
	_tabs->addTab(widget_gauss, "Gauss");

	setCentralWidget(_tabs);



	connect(_buttonCompute, SIGNAL(clicked()), this, SLOT(computation()));


}

MainWindow::~MainWindow()
{

}

void MainWindow::computation()
{
	_input1.clear();
	_input2.clear();

	_input1.push_back(_input1_11->text().toDouble());
	_input1.push_back(_input1_12->text().toDouble());
	_input1.push_back(_input1_21->text().toDouble());
	_input1.push_back(_input1_22->text().toDouble());

	_input2.push_back(_input2_11->text().toDouble());
	_input2.push_back(_input2_12->text().toDouble());
	_input2.push_back(_input2_21->text().toDouble());
	_input2.push_back(_input2_22->text().toDouble());


	auto start = std::chrono::steady_clock::now();

	size_t mat_size;
	mat_size = static_cast<size_t>(_mat_size->value());

	result(_input1, _input2, _output, mat_size);
	auto stop = std::chrono::steady_clock::now();
	double time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	qDebug() << "time: " << time;


	if(_output.size() > 3)
	{
		_output_11->setText(QString::number(_output[0]));
		_output_12->setText(QString::number(_output[1]));
		_output_21->setText(QString::number(_output[2]));
		_output_22->setText(QString::number(_output[3]));
	}
	else
		qDebug() << "wtf, output size must not be" << _output.size();

	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(-1e1,1e1);

	_input1_11->setText(QString::number(distribution(generator)));
	_input1_12->setText(QString::number(distribution(generator)));
	_input1_21->setText(QString::number(distribution(generator)));
	_input1_22->setText(QString::number(distribution(generator)));


	_input2_11->setText(QString::number(distribution(generator)));
	_input2_12->setText(QString::number(distribution(generator)));
	_input2_21->setText(QString::number(distribution(generator)));
	_input2_22->setText(QString::number(distribution(generator)));

}
