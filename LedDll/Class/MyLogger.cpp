#include "stdafx.h"
#include "MyLogger.h"
MyLogger *MyLogger::my_logger = NULL;

MyLogger::MyLogger()
{
	log4cplus::initialize();
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(MY_LOG_FILE_PATH));
	logger = Logger::getRoot();
}

MyLogger * MyLogger::getInstance()
{
	if (my_logger == NULL)
	{
		my_logger = new MyLogger();
	}
	return my_logger;
}


MyLogger::~MyLogger()
{
	if (my_logger)
	{
		delete my_logger;
		my_logger = NULL;
	}
}
