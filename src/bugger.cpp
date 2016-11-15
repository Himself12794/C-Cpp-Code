/*
 * bugger.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: phwhitin
 */

#include "bugger.h"
#include <iostream>

namespace buggered {

	bugger::bugger(int x) {
		this->x = x;
	}

	void bugger::do_bugger() {
		std::cout << "Bugger me" << std::endl << "Bugger you" << std::endl;
	}

	int bugger::do_math() {
		return x * x;
	}

	void bugger::set_x(int val) {
		x = val;
	}

	int bugger::get_x() {
		return x;
	}

	double line::compute_y(double val) {
		return (val * slope) + y_intercept;
	}

	line line::operator *(const double scalar) {
		return line { slope * scalar, y_intercept };
	}

	point line::intercept(line& other) {

		double x = (other.y_intercept - this->y_intercept)
				/ (this->slope - other.slope);
		double y = other.compute_y(x);

		return point { x, y };
	}
}
