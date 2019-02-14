#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>




class QGridLayout;
class QLineEdit;
class QLabel;
class QPushButton;
class QSpinBox;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QTabWidget * _tabs;

	QGridLayout * _grid;
	QPushButton * _buttonCompute;

	QLineEdit * _input1_11;
	QLineEdit * _input1_12;
	QLineEdit * _input1_21;
	QLineEdit * _input1_22;

	QLineEdit * _input2_11;
	QLineEdit * _input2_12;
	QLineEdit * _input2_21;
	QLineEdit * _input2_22;

	QSpinBox * _mat_size;

	QLabel * _output_11;
	QLabel * _output_12;
	QLabel * _output_21;
	QLabel * _output_22;


	std::vector<double> _input1;
	std::vector<double> _input2;
	std::vector<double> _output;

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void computation();
};

#endif // MAINWINDOW_H
