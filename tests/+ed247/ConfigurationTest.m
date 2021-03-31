classdef ConfigurationTest < matlab.unittest.TestCase
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function setFileFolder(testCase)
            rootdir = regexprep(mfilename('fullpath'),'\+.*','');
            testCase.filefolder_ = fullfile(rootdir,'_files');
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testConstructor(testCase)
            
            % [ SETUP ]
            filename = fullfile(testCase.filefolder_,'.metadata01');
            
            % [ EXERCISE ]
            f = @() ed247.Configuration(filename);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f)
            
        end
        
        function testDestructorNotDirty(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir(),'.metadataXX');
            if exist(filename,'file') == 2
                delete(filename)
            end
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            testCase.assertFalse(conf.IsDirty)
            delete(conf)
            
            % [ VERIFY ]
            testCase.verifyNotEqual(exist(filename,'file'),2)
            
            % [ TEARDOWN ]
            if exist(filename,'file') == 2
                delete(filename)
            end
            
        end
        
        function testDestructorDirty(testCase)
            
            % [ SETUP ]
            filename = fullfile(tempdir(),'.metadataXX');
            if exist(filename,'file') == 2
                delete(filename)
            end
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            conf.ED247 = tempdir;
            testCase.assertTrue(conf.IsDirty)
            delete(conf)
            
            % [ VERIFY ]
            testCase.verifyEqual(exist(filename,'file'),2)
            
            % [ TEARDOWN ]
            if exist(filename,'file') == 2
                delete(filename)
            end
            
        end
        
        function testGetAdapter(testCase)
            
            % [ SETUP ]
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.Adapter;
            
            % [ VERIFY ]
            testCase.verifyTrue(isdir(actual)) %#ok<ISDIR>
            
        end
        
        function testGetED247(testCase)
            
            % [ SETUP ]
            ed247_envvar = getenv(ed247.Configuration.ED247_FOLDER_VARIABLE);
            setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,ed247_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetED247(testCase)
            
            % [ SETUP ]
            ed247_envvar = getenv(ed247.Configuration.ED247_FOLDER_VARIABLE);
            setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,ed247_envvar));
            
            expected = tempdir;
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            conf.ED247 = expected;
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetED247HeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            ed247_envvar = getenv(ed247.Configuration.ED247_FOLDER_VARIABLE);
            setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.ED247_FOLDER_VARIABLE,ed247_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            filename = fullfile(tempdir,'testSetGetED247HeaderFileNonExistingFolder');
            
            % [ EXERCISE ]
            conf.ED247 = filename;
            actual = conf.ED247;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetLibXML2(testCase)
            
            % [ SETUP ]
            libxml2_envvar = getenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE);
            setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,libxml2_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetLibXML2(testCase)
            
            % [ SETUP ]
            libxml2_envvar = getenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE);
            setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,libxml2_envvar));
            
            expected = tempdir;
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            conf.LibXML2 = expected;
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetLibXML2HeaderFileNonExistingFolder(testCase)
            
            % [ SETUP ]
            libxml2_envvar = getenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE);
            setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.LIBXML2_FOLDER_VARIABLE,libxml2_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            filename = fullfile(tempdir,'testSetGetLibXML2HeaderFileNonExistingFolder');
            
            % [ EXERCISE ]
            conf.LibXML2 = filename;
            actual = conf.LibXML2;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetMEX(testCase)
            
            % [ SETUP ]
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.MEX;
            
            % [ VERIFY ]
            testCase.verifyTrue(isdir(actual)) %#ok<ISDIR>
            
        end
        
        function testGetMinGW(testCase)
            
            % [ SETUP ]
            mingw_envvar = getenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE);
            setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,mingw_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testSetGetMinGW(testCase)
            
            % [ SETUP ]
            mingw_envvar = getenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE);
            setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,mingw_envvar));
            
            expected = tempdir;
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            conf.MinGW = expected;
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testSetGetMinGWPathNonExistingFile(testCase)
            
            % [ SETUP ]
            mingw_envvar = getenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE);
            setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,mingw_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            invalidpath = fullfile(tempdir,'thisisanonexistingfolder');
            
            % [ EXERCISE ]
            conf.MinGW = invalidpath;
            actual = conf.MinGW;
            
            % [ VERIFY ]
            testCase.verifyEmpty(actual)
            
        end
        
        function testGetFilename(testCase)
            
            % [ SETUP ]
            filename = fullfile(testCase.filefolder_,'.metadata01');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            actual = conf.Filename;
            
            % [ VERIFY ]
            testCase.verifyEqual(actual,filename)
            
        end
        
        function testReload(testCase)
            
            % [ SETUP ]
            mingw_envvar = getenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE);
            setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,'');
            resetEnvVar = onCleanup(@() setenv(ed247.Configuration.MINGW_ENVIRONMENT_VARIABLE,mingw_envvar));
            
            filename = fullfile(testCase.filefolder_,'.metadata02');
            conf = ed247.Configuration(filename);
            
            % [ EXERCISE ]
            reload(conf)
            
            % [ VERIFY ]
            testCase.verifyEqual(conf.Name,         'ED247 for Simulink')
            testCase.verifyEqual(conf.Date,         '2020/12/18')
            testCase.verifyEqual(conf.Version,      '1.2.0')
            testCase.verifyEqual(conf.Developer,    'MathWorks Consulting Services')
            testCase.verifyEqual(conf.Maintainer,   'IYYSW')
            testCase.verifyEqual(conf.ED247,        'C:\Temp')
            testCase.verifyEqual(conf.LibXML2,      'C:\Temp')
            testCase.verifyEqual(conf.MinGW,        'C:\Temp')
            
        end
        
        function testSave(testCase)
            
            % [ SETUP ]
            ed247folder     = tempdir;
            libxml2folder   = tempdir;
            mingwfolder     = tempdir;
            
            original = fullfile(testCase.filefolder_,'.metadata01');
            filename = fullfile(tempdir(), 'testSave');
            copyfile(original,filename,'f')
            deleteFile = onCleanup(@() delete(filename));
            
            conf = ed247.Configuration(filename);
            
            conf.ED247      = ed247folder;
            conf.LibXML2    = libxml2folder;
            conf.MinGW      = mingwfolder;
            
            % [ EXERCISE ]
            save(conf)
            
            % [ VERIFY ]
            actual = fileread(filename);
            
            if ispc()
                platform = 'Windows';
            else
                platform = 'Linux';
            end
            
            platform_config = regexp(actual,sprintf('(?<=--- %s ---).*?(?=---)',platform),'match');
            platform_config = regexp(platform_config, '(\w+)=([^\n\r]+)','tokens');
            platform_config = vertcat(platform_config{1}{:});
            
            expected = { ...
                'ED247',    ed247folder; ...
                'LibXML2',  libxml2folder; ...
                'MinGW',    mingwfolder; ...
                };
            testCase.verifyEqual(platform_config,expected)
            
        end
        
    end
    
end