<?php

header("Content-Security-Policy: default-src 'self'; script-src 'self'");

echo "<h1>Ausgabe der Feldinhalte</h1>";

$name = isset($_POST["Name"]) ? htmlspecialchars($_POST["Name"]) : "Nicht angegeben";
$adresse = isset($_POST["Adresse"]) ? htmlspecialchars($_POST["Adresse"]) : "Nicht angegeben";
$email = isset($_POST["Email"]) ? htmlspecialchars($_POST["Email"]) : "Nicht angegeben";

echo "<p><strong>Name: </strong> $name</p>";
echo "<p><strong>Adresse: </strong> $adresse</p>";
echo "<p><strong>Email: </strong> $email</p>";

echo "<hr>";



?>