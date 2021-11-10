<b>This is console variation of Match3 game for Red Brix Wall company</b>

<b>Logic:</b>
<ul>
  <li>
    10x10 field, but it easily can be changed.
  </li>
  <li>
    Remove lines with 3 or more crystalls in a sequence
  </li>
  <li>
    After removing crystall, crystalls above should move down on free cells
  </li>
  <li>
    There is no special crystalls with bonus effect, but architecture is made for them in a future updates (NOTE: move() and onDestroy() methods should check a type of crystall)
  </li>
</ul>

<b>Interface:</b>
<ul>
  <li>
    Visualization of game is a console app, but it's easy to write any other graphic visualizator as its input data is only two dimensional array to render
  </li>
   <li>
     User input is a command such as <b>"c x y d"</b> where:
    <ul>
      <li>
        <b>c</b> - command to execute.
        <ul>
          <li>
            <b>m</b> - move crystall on x y in direction.
          </li>
          <li>
            <b>q</b> - quit an application.
          </li>
        </ul>
      </li>
      <li>
           <b>x, y</b> - coordinates in table. Note that <b>x</b> is a coord for a column(horizontal), and <b>y</b> is for a row(vertical).
          </li>
          <li>
            <b>d</b> - direction to move. Require only one letter, which is first letter of direction(left, right, up, down)
           </li>
    </ul>
   </li>
   
</ul>
