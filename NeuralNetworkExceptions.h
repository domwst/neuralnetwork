#ifndef NEURAL_NETWORK_EXCEPTIONS_H
#define NEURAL_NETWORK_EXCEPTIONS_H

#include <exception>
#include <cstring>

class InputVectorSizeError : public std::exception
{
private:

	char *message;


public:

	InputVectorSizeError(const char*);
	const char* what() const noexcept;
};

InputVectorSizeError::InputVectorSizeError(const char *s)
{
	int n = strlen(s);
	message = new char[n];
	strncpy(message, s, n);
}

const char* InputVectorSizeError::what() const noexcept
{
	return message;
}


class InitVectorSizeError : public std::exception
{
private:

	char *message;


public:

	InitVectorSizeError(const char*);
	const char* what() const noexcept;
};

InitVectorSizeError::InitVectorSizeError(const char *s)
{
	int n = strlen(s);
	message = new char[n];
	strncpy(message, s, n);
}

const char* InitVectorSizeError::what() const noexcept
{
	return message;
}


class OutputVectorSizeError : public std::exception
{
private:

	char *message;


public:

	OutputVectorSizeError(const char*);
	const char* what() const noexcept;
};

OutputVectorSizeError::OutputVectorSizeError(const char *s)
{
	int n = strlen(s);
	message = new char[n];
	strncpy(message, s, n);
}

const char* OutputVectorSizeError::what() const noexcept
{
	return message;
}

#endif // NEURAL_NETWORK_EXCEPTIONS_H