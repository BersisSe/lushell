import * as vscode from 'vscode';
import * as os from 'os';
import * as path from 'path';

export function activate(context: vscode.ExtensionContext) {
  const typingsPath = path.join(os.homedir(), '.lushell', 'lushell-typings', 'lushell.d.lua');
  const config = vscode.workspace.getConfiguration('Lua.workspace');
  const library = config.get<Array<string>>('library');
  if (!library?.includes(typingsPath)) {
    library?.push(typingsPath);
    config.update('library', library, vscode.ConfigurationTarget.Global)
      .then(() => {
        vscode.window.showInformationMessage(`Lushell typings have been added to Lua IntelliSense.`);
      }, (error) => {
        vscode.window.showErrorMessage(`Failed to update Lua workspace library: ${error}`);
      });
  }
}

export function deactivate() {}
