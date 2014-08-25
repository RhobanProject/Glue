$(document).ready(function(){
    function include(file) {
        $('head').append('<script type="text/javascript" src="demo/'+file+'"></script>');
    }
    
    blocks = new Blocks();

    for (var name in glue_blocks) {
        var block = glue_blocks[name];
        blocks.register(block);
    }

    for (var from_type in glue_convertibles) {
        for (var x in glue_convertibles[from_type]) {
            var to_type = glue_convertibles[from_type][x];
            blocks.types.addCompatibilityOneWay(from_type, to_type);
        }
    }

    blocks.run('#blocks');
    // blocks.load(scene);

    blocks.ready(function() {
	blocks.menu.addAction('Export', function(blocks) {
	    alert($.toJSON(blocks.export()));
	}, 'export');

        $('.setLabel').click(function() {
            for (k in blocks.edges) {
                var edge = blocks.edges[k];
                edge.setLabel('Edge #'+edge.id);
            }
        });

        $('.setInfos').click(function() {
            for (k in blocks.blocks) {
                var block = blocks.blocks[k];
                block.setInfos('Hello, I am the block #'+block.id);
            }
        });

        $('.setDescriptions').click(function() {
            for (k in blocks.blocks) {
                var block = blocks.blocks[k];
                block.setDescription('This is the block #'+block.id);
            }
        });

        $('.resize').click(function() {
            $('#blocks').width('300px');
            blocks.perfectScale();
        });

        $('.hideIcons').click(function() {
            blocks.showIcons = false;
            blocks.redraw();
        });

        $('.stressTest').click(function() {
            for (var x=0; x<1000; x+=100) {
                for (var y=0; y<1000; y+=100) {
                    blocks.addBlock('Sinus', x, y);
                }
            }
        });
    });
});
