#pragma once
#include <exception>
#include <string>

class NonExistentFile : public std::exception {

	const char* what() const noexcept {

		return "File couldn't be found!\n";
	}
};

class FileWontOpen : public std::exception {

	const char* what() const noexcept {

		return "File couldn't be opened!\n";
	}
};

class NotAppropriateFormat : public std::exception {

	const char* what() const noexcept {

		return "Destination format is not appropriate!\n";
	}
};

class FirstLayerNoDimension : public std::exception {

	const char* what() const noexcept {

		return "Dimension must be set for first layer!\n";
	}
};

class UnappropriateSize : public std::exception {

	const char* what() const noexcept {

		return "Size of image is unappropriate!\n";
	}
};

class IndexOutOfBounds : public std::exception {

	const char* what() const noexcept {

		return "Index out of bounds!\n";
	}
};

class NonExistentElement : public std::exception {

	const char* what() const noexcept {

		return "Element doesn't exist!\n";
	}
};

class SelectionAlreadyExists : public std::exception {

	const char* what() const noexcept {

		return "Selection with that name already exists!\n";
	}
};

class NotAppropriateNumber : public std::exception {

	const char* what() const noexcept {

		return "Number you tried operation with is incorrect!\n";
	}
};

class NoLayers : public std::exception {

	const char* what() const noexcept {

		return "There are no visible layers!\n";
	}
};

class SelectionDoesntExist : public std::exception {

	const char* what() const noexcept {

		return "Selection with that name doesn't exist!\n";
	}
};

class NoSuchOperation : public std::exception {

	const char* what() const noexcept {

		return "There is no such operation!\n";
	}
};