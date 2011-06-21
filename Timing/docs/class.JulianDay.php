<?


function  mod($a,$b)
{
   return floor( $a - floor($a/$b)*$b );
}
function  div($a,$b)
{
   return floor($a/$b);
}



class JulianDay {



 function  gregorianDateToJulianDay(&$jd,$d,$m,$y)
 {
    $Km = array(0,0,31,61,92,122,153,184,214,245,275,306,337);  
    $Ky = 365;
    $Kc = 365*100 + 24; // 36524
    $Kq = 365*400 + 97; // 146097

    $m -= 2; // Correct month
    if ($m<1) {$m+=12;$y-=1;}
   
    $jd  = $Kq * div($y,400)
         + $Kc * div(mod($y,400),100)
         + 365 * mod($y,100)
         + div(mod($y,100),4)
         + $Km[$m]
         + $d
         + 1721081; // - For real julian day
 }


 function  julianDayToGregorianDate($jd,&$d,&$m,&$y)
 {
    $Km = array(0, 0,31,61, 92,122,153, 184,214,245, 275,306,337);  
    $Nm = array(0, 31,30,31, 30,31,31,  30,31,30,    31,31,29);  
    $Ky = 365;
    $K4 = 365*4 + 1; // 1461
    $Kc = 365*100 + 24; // 36524
    $Kq = 365*400 + 97; // 146097

    $jd -= 1721082;
    $y   = div( $jd, $Kq) * 400; 
    $jd  = mod( $jd, $Kq );
    if ($jd==$Kq-1) {
       $y += 400;
       $m  = 2;
       $d  = 29;
       return;
    }
    $y  += div( $jd, $Kc) * 100; 
    $jd  = mod( $jd, $Kc );
    $y  += div( $jd, $K4) * 4;
    $jd  = mod( $jd, $K4 );
    if ($jd==$K4-1) {
       $y += 4;
       $m  = 2;
       $d  = 29;
       return;
    }
    $y  += div( $jd, $Ky);
    $jd  = mod( $jd, $Ky);
    $m   = div( $jd, 31) + 1; 
    $d   = $jd - $Km[$m] + 1;
    if ($Nm[$m]<$d) {
       $d -= $Nm[$m];
       $m++;
    }
    $m += 2; // correct date
    if ($m>12) {
       $m -= 12;
       $y++;
    }
 }


}

