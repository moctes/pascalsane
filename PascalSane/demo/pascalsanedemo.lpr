program pascalsanedemo;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms
  { you can add units after this }, demomain;

begin
  Application.Title := 'Libsane Demo';
  Application.Initialize;
  Application.CreateForm ( TScannerForm, ScannerForm ) ;
  Application.Run;
end.

