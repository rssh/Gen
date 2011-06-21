<? // My julian numbers test algorythm

include "class.JulianDay.php";
$C = new JulianDay;

// Here are the program:

$begin_year = 495; // Set this variables
$end_year = 1000;

$months_normal = array (0,31,28, 31,30,31, 30,31,31, 30,31,30, 31);
$months_lap = array (0,31,29, 31,30,31, 30,31,31, 30,31,30, 31);
$months = &$months_normal;
$C->gregorianDateToJulianDay($jd,31,12,$begin_year-1);
$jd1 = $jd;
$bad = false;
$d = 0; $m = 0; $y = 0;
$d1 = 0; $m1 = 0; $y1 = 0;


for ($y=$begin_year;$y<$end_year;$y++) {
   echo "<BR>Year [$y]";

   if (mod($y,4)==0) {
      if (mod($y,100)!=0) {
         $months = &$months_lap;
      } elseif (mod($y,400)==0) {
         $months = &$months_lap;
      } else {
         $months = &$months_normal;
      }
   } else {
      $months = &$months_normal;
   }

   for($m=1;$m<13;$m++){
      echo "<BR>Month [$m] has ".$months[$m]." days ";
      for($d=1;$d<($months[$m]+1);$d++){
         $C->gregorianDateToJulianDay($jd1,$d,$m,$y);
         $C->JulianDayToGregorianDate($jd1,$d1,$m1,$y1);
//         echo "<BR>Day [$d] : $jd1 ";
         if( ($jd1-$jd)!=1 || $d!=$d1 || $m!=$m1 || $y!=$y1 ){
            echo "[$d.$m.$y] is BAD!!!"; $bad = true; break 3;
         };$jd=$jd1;
      }
   }
}

if ($bad) {
   echo "<BR>FAILED: There is bad date.";
} else {
   echo "<BR>OK: There is no bad dates.";
}



?>

