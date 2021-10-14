classdef Dependencies < matlab.mixin.SetGet
   
    %% CONSTANT
    properties (Constant)
        ARTIFACT_JOB = "compile";
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        HostLibraryURL
        TargetLibraryURL
    end
    
    %% OPTIONS
    properties
        BranchName      (1,1)   string
        Host            (1,1)   string
        ProjectID       (1,1)   string
        ServerURL       (1,1)   string
        Target          (1,1)   string
        Token           (1,1)   string
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        
        installationfolder_     (1,1)   string
        rootfolder_             (1,1)   string
        temporaryfolder_        (1,1)   string
        
        hostarchive_            (1,1)   string
        targetarchive_          (1,1)   string
        
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = Dependencies(rootfolder, installationfolder, temporaryfolder, varargin)
                        
            obj.installationfolder_ = installationfolder;
            obj.rootfolder_         = rootfolder;
            obj.temporaryfolder_    = temporaryfolder;
            
            obj.hostarchive_        = fullfile(obj.temporaryfolder_, "host.zip");
            obj.targetarchive_      = fullfile(obj.temporaryfolder_, "target.zip");
            
            if ~isempty(varargin)
                set(obj, varargin{:})
            end
            
            if ~isfolder(obj.temporaryfolder_)
                mkdir(obj.temporaryfolder_)
            end
            
        end
        
        function delete(obj)
            if isfolder(obj.temporaryfolder_)
                rmdir(obj.temporaryfolder_,"s")
            end
        end
        
    end
    
    %% ACCESSORS
    methods
               
        function hostlibraryurl = get.HostLibraryURL(obj)
            % Reference : https://docs.gitlab.com/ee/api/job_artifacts.html
            hostlibraryurl = obj.ServerURL + "/api/v4/projects/" + obj.ProjectID + "/jobs/artifacts/" + obj.BranchName + "/download?job=" + obj.Host + ":" + obj.ARTIFACT_JOB;
        end
       
        function targetlibraryurl = get.TargetLibraryURL(obj)
            % Reference : https://docs.gitlab.com/ee/api/job_artifacts.html
            targetlibraryurl = obj.ServerURL + "/api/v4/projects/" + obj.ProjectID + "/jobs/artifacts/" + obj.BranchName + "/download?job=" + obj.Target + ":" + obj.ARTIFACT_JOB;
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function download(obj)
                        
            opts = weboptions("HeaderFields",["PRIVATE-TOKEN",obj.Token],"ContentType","binary");
                        
            try
                obj.print("Download Host archive at URL '%s' to '%s' ...", obj.HostLibraryURL, obj.hostarchive_)
                websave(obj.hostarchive_,obj.HostLibraryURL,opts);
                obj.print("\b Done")
            catch me
                
                if me.identifier == "MATLAB:webservices:HTTP404StatusCodeError"
                    obj.warning("ci:Prepare:unableToAccessURL", ...
                        "Cannot download Host archive (%s) : %s", me.identifier, me.message)
                else
                    obj.rethrow(me)
                end
                
            end
                    
            try
                obj.print("Download Target archive at URL '%s' to '%s' ...", obj.TargetLibraryURL, obj.targetarchive_)
                websave(obj.targetarchive_,obj.TargetLibraryURL,opts);
                obj.print("\b Done")
            catch me
                
                if me.identifier == "MATLAB:webservices:HTTP404StatusCodeError"
                    obj.warning("ci:Prepare:unableToAccessURL", ...
                        "Cannot download Target archive (%s) : %s", me.identifier, me.message)
                else
                    obj.rethrow(me)
                end
                
            end
            
        end
        
        function install(obj)
            
            obj.assert(exist(obj.hostarchive_, "file") == 2, ...
                "ci:Prepare:NoArchive", "Host archive (%s) does not exist, make sure to download it before installing", ...
                obj.hostarchive_)
            obj.assert(exist(obj.targetarchive_, "file") == 2, ...
                "ci:Prepare:NoArchive", "Target archive (%s) does not exist, make sure to download it before installing", ...
                obj.targetarchive_)
            
            currentfolder = pwd;
            cd(obj.temporaryfolder_)
            pushd = onCleanup(@() cd(currentfolder));
            
            obj.print("Unzip host archive to temporary folder")
            unzip(obj.hostarchive_)
            obj.print("Unzip target archive to temporary folder")
            unzip(obj.targetarchive_)
            
            ed247installationfolder = fullfile(obj.installationfolder_, "ED247_LIBRARY");
            if ~isfolder(ed247installationfolder)
                obj.print("Create folder '%s'", ed247installationfolder)
                mkdir(ed247installationfolder)
            end
            
            %
            % Copy host library (Linux or Windows) to dependency folder
            %
            hostlibraryfiles   = fullfile(obj.temporaryfolder_, "ED247_LIBRARY", "_install");
            hostlibraryinstall = fullfile(ed247installationfolder, "host");
            if ~isfolder(hostlibraryinstall)
                obj.print("Create folder '%s'", hostlibraryinstall)
                mkdir(hostlibraryinstall)
            end
            obj.print("Copy ED247 host library into '%s'", hostlibraryinstall)
            copyfile(fullfile(hostlibraryfiles, "*.*"), hostlibraryinstall)
            
            %
            % Copy target library (QNX) to dependency folder
            %
            targetlibraryfiles      = fullfile(obj.temporaryfolder_, "ED247_LIBRARY", "_install_qnx");
            targetlibraryinstall    = fullfile(ed247installationfolder, "target");
            if ~isfolder(targetlibraryinstall)
                obj.print("Create folder '%s'", targetlibraryinstall)
                mkdir(targetlibraryinstall)
            end
            obj.print("Copy ED247 target library into '%s'", targetlibraryinstall)
            copyfile(fullfile(targetlibraryfiles, "*.*"), targetlibraryinstall)
            
            %
            % Other dependencies (LibXML2)
            %
            libxml2installationfolder = fullfile(obj.installationfolder_, "LibXML2");
            if ~isfolder(libxml2installationfolder)
                obj.print("Create folder '%s'", libxml2installationfolder)
                mkdir(libxml2installationfolder)
            end
            
            libxml2archive = fullfile(obj.rootfolder_, "archives", "libxml2.zip");
            obj.print("Extract LibXML2 into '%s'", libxml2installationfolder)
            unzip(libxml2archive, libxml2installationfolder)
            
        end
        
        function configure(obj)
           
            config = ed247.Configuration.default();
            
            config.ED247    = fullfile(obj.installationfolder_, "ED247_LIBRARY");
            config.LibXML2  = fullfile(obj.installationfolder_, "LibXML2");
                        
            save(config)
            
        end
        
    end
    
    %% HELPER METHODS (PROTECTED)
    methods (Access = protected)
        
        function assert(obj,varargin) %#ok<INUSL>
            assert(varargin{:})
        end
        
        function error(obj,varargin) %#ok<INUSL>
            error(varargin{:})
        end
        
        function warning(obj,varargin) %#ok<INUSL>
            warning(varargin{:})
        end
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1,varargin{1} + "\n",varargin{2:end})
        end
        
        function rethrow(obj,varargin) %#ok<INUSL>
            rethrow(varargin{:})
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
       
        function varargout = runOnLocal(varargin)
                   
            proj = ci.openProject();
            
            if ispc()
                host = "windows";
            else
                host = "linux";
            end
            
            installationfolder = fullfile(proj.RootFolder, "deps");
            tempfolder = fullfile(tempdir, "ED247Cache");
            
            obj = ci.Dependencies( proj.RootFolder, installationfolder, tempfolder, ...
                "BranchName",   "master",                       ...
                "Host",         host,                           ...
                "ProjectID",    "314",                          ...
                "ServerURL",    "http://gnb-csg-master:8484",   ...
                "Target",       "qnx",                          ...
                "Token",        "TGyAPFRAZzRQAzyBNTxB"          ...
                );
                        
            if nargout
                varargout = {obj};
            end
            
        end
        
        function runOnGitLabCI(varargin)
           
            status = -1;
            
            try
                
                proj = ci.openProject();
                
                if ispc()
                    host = "windows";
                else
                    host = "linux";
                end
                
                installationfolder = fullfile(proj.RootFolder, "deps");
                tempfolder = fullfile(tempdir, "ED247Cache");
                
                branchname = getenv("ARTIFACT_PROJECT_BRANCH");
                projectid  = getenv("ARTIFACT_PROJECT_ID");
                token      = getenv("ARTIFACT_PROJECT_TOKEN");
                
                obj = ci.Dependencies( proj.RootFolder, installationfolder, tempfolder, ...
                    "BranchName",   branchname,                     ... 
                    "Host",         host,                           ...
                    "ProjectID",    projectid,                      ... 
                    "ServerURL",    "http://gnb-csg-master:8484",   ...
                    "Target",       "qnx",                          ...
                    "Token",        token                           ... 
                    );
                
                download(obj)
                install(obj)
                configure(obj)
                
                status = 0;
                
            catch me
                disp(me.getReport())
            end
            
            quit(status)
            
        end
        
    end
    
end