<?php
function StoreInFile($tmp,$hum)
{
	$file = 'curr_readings.txt'; //No database yet, just use txt file
    $current = $tmp . "°C, " . $hum . "% humidity";
	file_put_contents($file, $current, LOCK_EX);
}


$temp = $_GET['temp'];
$humid = $_GET['humid'];



if (!isset($temp)) 
{
echo "
Failure: missing temp
";
exit;
}

if (!isset($humid)) 
{
echo "
Failure: missing humid
";
exit;
}

StoreInFile($temp, $humid);
echo "OK\n";
?>