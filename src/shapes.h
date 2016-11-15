/*
 * shapes.h
 *
 *  Created on: Aug 17, 2016
 *      Author: phwhitin
 */

#ifndef SHAPES_H_
#define SHAPES_H_


namespace shapes {

	class shape {
		protected:
			int height, width;
		public:
			shape( int a = 0, int b = 0) {
				width = a;
				height = b;
			}
			virtual ~shape() {}
			virtual int getArea() = 0;

	};

	class rectangle : public shape {

		public:
			rectangle( int a = 0, int b = 0 ) : shape( a, b) {}
			int getArea() {
				return height * width;
			}

	};

	class square : public rectangle {

		public:
			square( int w = 0 ) : rectangle( w, w ) {}

	};

	class triangle : public shape {

		public:
			triangle( int a = 0, int b = 0 ) : shape( a, b ) {}
			int getArea() {
				return (height * width) / 2;
			}

	};

}


#endif /* SHAPES_H_ */
