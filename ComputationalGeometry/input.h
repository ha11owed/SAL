#pragma once

class MouseKeyboardInput {
public:
	enum KeyType
	{
		None = 0,
		// Keyboard
		Ctrl = 1,
		Shift = 1 << 1,
		ArrowUp = 1 << 2,
		ArrowDown = 1 << 3,
		ArrowLeft = 1 << 4,
		ArrowRight = 1 << 5,
		// Mouse stuff
		MouseLeftPressed = 1 << 24,
		MouseLeftReleased = 1 << 25,
		MouseMiddlePressed = 1 << 26,
		MouseMiddleReleased = 1 << 27,
		MouseRightPressed = 1 << 28,
		MouseRightReleased = 1 << 29,
	};

	MouseKeyboardInput() : keyType(None), key(0), x(-1), y(-1) {}
	MouseKeyboardInput(unsigned char k, int mouseX, int mouseY) : keyType(None), key(k), x(mouseX), y(mouseY) {}
	MouseKeyboardInput(KeyType kType, int mouseX, int mouseY) : keyType(kType), key(0), x(mouseX), y(mouseY) {}

	KeyType keyType;
	unsigned char key;

	int x;
	int y;

	bool has(KeyType kt) const;
	bool isMouse() const;
};


// ---- Inline implementation ----
inline bool MouseKeyboardInput::has(MouseKeyboardInput::KeyType kt) const {
	return (keyType & kt) == kt;
}

inline bool MouseKeyboardInput::isMouse() const {
	return 0 != (keyType & (MouseLeftPressed | MouseLeftReleased | MouseMiddlePressed | MouseMiddleReleased | MouseRightPressed | MouseRightReleased));
}
