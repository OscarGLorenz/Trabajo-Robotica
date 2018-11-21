%% 3D Model Demo
% This is short demo that loads and renders a 3D model of a human femur. It
% showcases some of MATLAB's advanced graphics features, including lighting and
% specular reflectance.

% Copyright 2011 The MathWorks, Inc.


%% Load STL mesh
% Stereolithography (STL) files are a common format for storing mesh data. STL
% meshes are simply a collection of triangular faces. This type of model is very
% suitable for use with MATLAB's PATCH graphics object.
close all
% Import an STL mesh, returning a PATCH-compatible face-vertex structure
ax = axes('XLim',[-20 20],'YLim',[-20 20],'ZLim',[-20 20]);
hold(ax,'on');
fv = stlread('Patin.stl');
f = fv.vertices;     
     % Add a camera light, and tone down the specular highlighting
camlight('headlight');
grid on


view(3);

for t=linspace(0,2*pi)

G = [1 0 0; 0 cos(t) -sin(t);0 sin(t) cos(t);];
T = [t 0 0];
fv.vertices = (G*f')'+T;

%% Render
% The model is rendered with a PATCH graphics object. We also add some dynamic
% lighting, and adjust the material properties to change the specular
% highlighting.


p = patch(fv,'FaceColor',       [0.8 0.8 1.0], ...
         'EdgeColor',       'none',        ...
         'FaceLighting',    'gouraud',     ...
         'AmbientStrength', 0.15);
material('metal');


pause(0.1);
delete(p);

end


