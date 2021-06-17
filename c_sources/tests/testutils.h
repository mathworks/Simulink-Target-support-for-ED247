
 #ifndef __ED247TESTUTILS__
 #define __ED247TESTUTILS__

 #include "unistd.h"

 #include "gtest/gtest.h"

 class FileFolderTest : public ::testing::Test {
	 
	 protected:
		 virtual void SetUp();
		 
		 std::string filefolder_;
 };

 class A429Test : public FileFolderTest {};
 class A664Test : public FileFolderTest {};
 class A825Test : public FileFolderTest {};
 class NADTest : public FileFolderTest {};
 class IssuesTest : public FileFolderTest {};

 class ConfigurationTest : public FileFolderTest {};
 class ToolsTest : public FileFolderTest {};
 class TransmissionTest : public FileFolderTest {};

 #endif