<?php
//Este código esta basado en un proyecto de Rui Santos de la pagina https://RandomNerdTutorials.com

// Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files.

$servername = "localhost";

// REPLACE with your Database name
$dbname = "mons";
// REPLACE with Database user
$username = "root";
// REPLACE with Database user password
$password = "";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "48b45ce1fdb2293";

$api_key= $table = $fecha = $pm1p0_sp = $pm2p5_sp = $pm10p0_sp = $n0p3 = $n0p5 = $n1p0 = $n2p5 = $n5p0 = $n10p0  = "";
 


if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $fecha = test_input($_POST["fecha"]);
        $table = test_input($_POST["table"]);
        		
		$pm1p0_sp = test_input($_POST["pm1p0_sp"]);
		$pm2p5_sp = test_input($_POST["pm2p5_sp"]);
		$pm10p0_sp = test_input($_POST["pm10p0_sp"]);
		
		$n0p3 = test_input($_POST["n0p3"]);
		$n0p5 = test_input($_POST["n0p5"]);
		$n1p0 = test_input($_POST["n1p0"]);
		$n2p5 = test_input($_POST["n2p5"]);
		$n5p0 = test_input($_POST["n5p0"]);
		$n10p0 = test_input($_POST["n10p0"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql_1 = "INSERT INTO m1 (fecha, pm1p0_sp, pm2p5_sp, pm10p0_sp, n0p3, n0p5, n1p0, n2p5, n5p0, n10p0)
        VALUES ('" . $fecha . "', '" . $pm1p0_sp . "', '" . $pm2p5_sp . "', '" . $pm10p0_sp . "', '" . $n0p3 . "', '" . $n0p5 . "', '" . $n1p0 . "', '" . $n2p5 . "', '" . $n5p0 . "', '" . $n10p0 . "')";
		
		$sql_2 = "INSERT INTO m2 (fecha, pm1p0_sp, pm2p5_sp, pm10p0_sp, n0p3, n0p5, n1p0, n2p5, n5p0, n10p0)
        VALUES ('" . $fecha . "', '" . $pm1p0_sp . "', '" . $pm2p5_sp . "', '" . $pm10p0_sp . "', '" . $n0p3 . "', '" . $n0p5 . "', '" . $n1p0 . "', '" . $n2p5 . "', '" . $n5p0 . "', '" . $n10p0 . "')";
		
		$sql_3 = "INSERT INTO m3 (fecha, pm1p0_sp, pm2p5_sp, pm10p0_sp, n0p3, n0p5, n1p0, n2p5, n5p0, n10p0)
        VALUES ('" . $fecha . "', '" . $pm1p0_sp . "', '" . $pm2p5_sp . "', '" . $pm10p0_sp . "', '" . $n0p3 . "', '" . $n0p5 . "', '" . $n1p0 . "', '" . $n2p5 . "', '" . $n5p0 . "', '" . $n10p0 . "')";
      	
		$sql_4 = "INSERT INTO m4 (fecha, pm1p0_sp, pm2p5_sp, pm10p0_sp, n0p3, n0p5, n1p0, n2p5, n5p0, n10p0)
        VALUES ('" . $fecha . "', '" . $pm1p0_sp . "', '" . $pm2p5_sp . "', '" . $pm10p0_sp . "', '" . $n0p3 . "', '" . $n0p5 . "', '" . $n1p0 . "', '" . $n2p5 . "', '" . $n5p0 . "', '" . $n10p0 . "')";
      	
		$sql_5 = "INSERT INTO m5 (fecha, pm1p0_sp, pm2p5_sp, pm10p0_sp, n0p3, n0p5, n1p0, n2p5, n5p0, n10p0)
        VALUES ('" . $fecha . "', '" . $pm1p0_sp . "', '" . $pm2p5_sp . "', '" . $pm10p0_sp . "', '" . $n0p3 . "', '" . $n0p5 . "', '" . $n1p0 . "', '" . $n2p5 . "', '" . $n5p0 . "', '" . $n10p0 . "')";
      	
     if ($table=='m1'){

		 if ($conn->query($sql_1) === TRUE) {
				echo "New record created successfully on Table 1";
			} 
			else {
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
	 }
    else if ($table=='m2'){

		 if ($conn->query($sql_2) === TRUE) {
				echo "New record created successfully on Table 2";
			} 
			else {
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
	 }
	 else if ($table=='m3'){

		 if ($conn->query($sql_3) === TRUE) {
				echo "New record created successfully on Table 3";
			} 
			else {
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
	 }
	 else if ($table=='m4'){

		 if ($conn->query($sql_4) === TRUE) {
				echo "New record created successfully on Table 4";
			} 
			else {
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
	 }
	 else if ($table=='m5'){

		 if ($conn->query($sql_5) === TRUE) {
				echo "New record created successfully on Table 5";
			} 
			else {
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
	 }
	else {echo "It doesn´t exist that table";}
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
