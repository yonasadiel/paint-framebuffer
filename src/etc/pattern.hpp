#ifndef PATTERN
#define PATTERN

#include "../drawable/image.hpp"

class PatternFactory {
public:
	const int totalImage = 7;
	Image **pat;
    
	PatternFactory() {
		pat = new Image*[totalImage];
		pat[0] = new Image("images/patterns/p001.pat");
		pat[1] = new Image("images/patterns/p002.pat");
		pat[2] = new Image("images/patterns/p003.pat");
		pat[3] = new Image("images/patterns/p004.pat");
		pat[4] = new Image("images/patterns/p005.pat");
		pat[5] = new Image("images/patterns/p006.pat");
		pat[6] = new Image("images/patterns/p007.pat");
	}
};

#endif