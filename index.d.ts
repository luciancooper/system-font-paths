// Type definitions for system-font-paths
// Project: https://github.com/luciancooper/system-font-paths
// Definitions by: Lucian Cooper <https://github.com/luciancooper>

/**
 * Fetches the paths for all locally installed fonts
 */
declare function systemFontPaths(): Promise<string[]>;

export = systemFontPaths;