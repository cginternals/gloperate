var plugins = pluginManager.plugins();
var currentPlugin = -1;

var nextPlugin = function()
{
    currentPlugin = (currentPlugin + 1) % plugins.length;
    viewer.loadPainter(plugins[currentPlugin].Name);
};

nextPlugin();
timer.start(5000, nextPlugin);
