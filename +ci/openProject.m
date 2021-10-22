function varargout = openProject(varargin)

folder = pwd;

proj = slproject.getCurrentProjects();
if ~isempty(proj)
    proj = currentProject();
else
    proj = openProject(folder);
end

if nargout
    varargout = {proj};
end