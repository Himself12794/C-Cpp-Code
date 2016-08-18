/*
 * bugger.h
 *
 *  Created on: Aug 16, 2016
 *      Author: phwhitin
 */

#include <iostream>

#ifndef BUGGER_H_
#define BUGGER_H_

namespace buggered {

	/**
	 * For testing purposes
	 *
	 */
	class bugger {

		public:
			void do_bugger();
			int do_math();
			void set_x(int x);
			int get_x();
			bugger(int x);

		private:
			int x;
	};

	struct point {
		double x;
		double y;
	};

	inline std::ostream & operator<<(std::ostream& str, const point& v) {
	  str << '(' << v.x << ',' << v.y << ')';
	  return str;
	}

	/**
	 * Represents a line.
	 *
	 */
	struct line {

		double slope;
		double y_intercept;

		double compute_y(double x);
		line operator *(const double scalar);
		point intercept(line &other);

	};

	inline std::ostream& operator<<(std::ostream& str, const line& v) {
	  str << "y=" << v.slope << "x" << (v.y_intercept > 0 ? '-' : '+') << v.y_intercept;
	  return str;
	}

	template< typename T >
	struct supports_optimized_implementation {
		static const bool value = false;
	};

	template<>
	struct supports_optimized_implementation< void > {
		static const bool value = true;
	};

	template< bool b >
	struct algorithm_selector {
		template< typename T >
		static void implementation( T& object ) {
			std::cout << "boo!" << std::endl;
		}
	};

	template<>
	struct algorithm_selector< true > {
		template< typename T >
		static void implementation( T& object ) {
			object.optimised_implementation();
		}
	};

	template< typename T >
	void algorithm( T& object ) {
		algorithm_selector< supports_optimized_implementation< T >::value >::implementation(object);
	}

	class object_a {};

	class object_b {
		public:
			inline void optimised_implementation() {
				std::cout << "blah" << std::endl;
			}
	};

	template<>
	struct supports_optimized_implementation< object_b > {
		static const bool value = true;
	};
}

#endif /* BUGGER_H_ */
