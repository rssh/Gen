<?

include "class.JulianDay.php";

// Protect variables:
if (!isset($compute)) {
   $compute='';
   $computations =' Enter data and make choice ';
} else {
   $computations =' Executed : '.$compute;
}
if (!isset($Y)) {$Y=0;echo "Set Year. ";}
if (!isset($M)) {$M=0;echo "Sset Month. ";}
if (!isset($D)) {$D=0;echo "Set Day. ";}
if (!isset($JD)) {$JD=0;echo "Set Julian Day. ";}
$C = new JulianDay;

// Compute:
if ($compute==' Convert to gregorian date ') {
   $C->julianDayToGregorianDate($JD,$D,$M,$Y);
} elseif ($compute==' Convert to julian day ') {
   $C->gregorianDateToJulianDay($JD,$D,$M,$Y);
};

// Output variables:
echo "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\"
        \"http://www.w3.org/TR/REC-html40/loose.dtd\">
<HTML>
<HEAD>
	<TITLE> Julian Day - Gregorian Date </TITLE>
</HEAD>

<BODY>

<TABLE border=0 cellpadding=0 cellspacing=0 width=\"100%\" height=\"100%\" align=center valign=center>
 <TR>
  <TD align=center valign=center>

   <TABLE border=0 cellpadding=0 cellspacing=3>
    <TR>
     <TD align=left valign=center>
      <FORM name=\"Broker\" action=\"julian.php\" method=\"post\">
       <INPUT type=submit name=compute value=\" Convert to julian day \"><BR>
       Gregorian date<BR>
       Year
       <INPUT type=text name=\"Y\" size=\"60\" value=\"$Y\"><BR>
       Month
       <INPUT type=text name=\"M\" size=\"60\" value=\"$M\"><BR>
       Day
       <INPUT type=text name=\"D\" size=\"60\" value=\"$D\"><BR>
       <INPUT type=submit name=compute value=\" Convert to gregorian date \"><BR>
       Julian day
       <INPUT type=text name=\"JD\" size=\"60\" value=\"$JD\"><BR>
       $computations
      </FORM>
     </TD></TR></TABLE>

  </TD></TR></TABLE>
  
</BODY>
</HTML>";

?>