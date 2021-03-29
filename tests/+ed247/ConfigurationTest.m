classdef ConfigurationTest < matlab.unittest.TestCase
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filename_
    end
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function setFilename(testCase)
            testCase.filename_ = fullfile(tempdir,'ED247ConfigurationTest.mat');
        end
        
    end
    
    %% METHODS TEARDOWN
    methods (TestMethodTeardown)
        
        function deleteFile(testCase)
            if exist(testCase.filename_,'file') == 2
                delete(testCase.filename_)
            end
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testConstructor(testCase)
            
            % [ EXERCISE ]
            f = @() ed247.Configuration(testCase.filename_);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f)
            
        end
        
        function testDestructorNotDirty(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            testCase.assumeNotEqual(exist(testCase.filename_,'file'),2)
            
            % [ EXERCISE ]
            testCase.assertFalse(conf.IsDirty)
            delete(conf)
            
            % [ VERIFY ]
            testCase.verifyNotEqual(exist(testCase.filename_,'file'),2)
            
        end
        
        function testDestructorDirty(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            testCase.assumeNotEqual(exist(testCase.filename_,'file'),2)
            
            % [ EXERCISE ]
            conf.Adapter = tempdir;
            testCase.assertTrue(conf.IsDirty)
            delete(conf)
            
            % [ VERIFY ]
            testCase.verifyEqual(exist(testCase.filename_,'file'),2)
            
        end
        
        function testGetAdapter(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.Adapter;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetAdapter(testCase)
            
            % [ SETUP ]
            expected = tempdir;
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.Adapter = expected;
            actual = conf.Adapter;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetAdapterHeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir,'testSetGetAdapterHeaderFileNonExistingFolder');
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.Adapter = filename;
            actual = conf.Adapter;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetED247(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetED247(testCase)
            
            % [ SETUP ]
            expected = tempdir;
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.ED247 = expected;
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetED247HeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir,'testSetGetED247HeaderFileNonExistingFolder');
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.ED247 = filename;
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetLibXML2(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetLibXML2(testCase)
            
            % [ SETUP ]
            expected = tempdir;
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.LibXML2 = expected;
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetLibXML2HeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir,'testSetGetLibXML2HeaderFileNonExistingFolder');
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.LibXML2 = filename;
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetMEX(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.MEX;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetMEX(testCase)
            
            % [ SETUP ]
            expected = tempdir;
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.MEX = expected;
            actual = conf.MEX;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetMEXHeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir,'testSetGetMEXHeaderFileNonExistingFolder');
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.MEX = filename;
            actual = conf.MEX;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetMinGW(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetMinGW(testCase)
            
            % [ SETUP ]
            expected = tempdir;
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.MinGW = expected;
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetMinGWPathNonExistingFile(testCase)
            
            % [ SETUP ]
            invalidpath = fullfile(tempdir,'thisisanonexistingfolder');
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            conf.MinGW = invalidpath;
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetFilename(testCase)
            
            % [ SETUP ]
            conf = ed247.Configuration(testCase.filename_);
            
            % [ EXERCISE ]
            actual = conf.Filename;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,testCase.filename_)
            
        end
        
        function testReload(testCase)
            
            % [ SETUP ]
            adapterfolder   = tempdir;
            ed247folder     = tempdir;
            libxml2folder   = tempdir;
            mexfolder       = tempdir;
            mingwfolder     = tempdir;
            
            conf = ed247.Configuration(testCase.filename_);
            data = struct('Adapter', adapterfolder, 'ED247', ed247folder, 'LibXML2', libxml2folder, 'MEX', mexfolder, 'MinGW', mingwfolder); %#ok<NASGU>
            save(testCase.filename_,'-struct','data')
            
            % [ EXERCISE ]
            reload(conf)
            
            % [ VERIFY ]
            testCase.verifyEqual(conf.Adapter,  adapterfolder)
            testCase.verifyEqual(conf.ED247,    ed247folder)
            testCase.verifyEqual(conf.LibXML2,  libxml2folder)
            testCase.verifyEqual(conf.MEX,      mexfolder)
            testCase.verifyEqual(conf.MinGW,    mingwfolder)
            
        end
        
        function testSave(testCase)
            
            % [ SETUP ]
            adapterfolder   = tempdir;
            ed247folder     = tempdir;
            libxml2folder   = tempdir;
            mexfolder       = tempdir;
            mingwfolder     = tempdir;
            
            conf = ed247.Configuration(testCase.filename_);
            conf.Adapter    = adapterfolder;
            conf.ED247      = ed247folder;
            conf.LibXML2    = libxml2folder;
            conf.MEX        = mexfolder;
            conf.MinGW      = mingwfolder;
            
            % [ EXERCISE ]
            save(conf)
            
            % [ VERIFY ]
            if ispc()
                platform = 'Windows';
            else
                platform = 'Linux';
            end
            expected = struct('Platform',platform,'Adapter', adapterfolder, 'ED247', ed247folder, 'LibXML2', libxml2folder, 'MEX', mexfolder, 'MinGW', mingwfolder);
            actual = load(testCase.filename_);
            actual = actual.configuration;
            testCase.verifyEqual(actual,expected)
            
        end
        
    end
    
end