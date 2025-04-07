$(document).ready(function(){
    // Configuración del toggle de la bomba de agua
    $('#toggleBomba').change(function() {
      if ($(this).prop('checked')) {
        // Si el toggle está activado, encender la bomba
        $.get("/bomba/encender", function(response) {
          console.log("Bomba encendida: " + response);
        });
      } else {
        // Si el toggle está desactivado, apagar la bomba
        $.get("/bomba/apagar", function(response) {
          console.log("Bomba apagada: " + response);
        });
      }
    });
  
    // Actualización del estado del toggle en base al estado de la bomba
    function actualizarEstadoBomba() {
      $.get("/bomba/estado", function(response) {
        if (response === "encendida") {
          $('#toggleBomba').prop('checked', true);
        } else {
          $('#toggleBomba').prop('checked', false);
        }
      });
    }
  
    // Comprobar el estado de la bomba cada 2 segundos
    setInterval(actualizarEstadoBomba, 2000);
  });
  