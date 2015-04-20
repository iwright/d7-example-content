<?php

/**
 * @file
 * Class definition for the basic page example content import.
 */

/**
 * Class basicNode
 */
class basicNode extends Migration {
  public function __construct($arguments) {
    parent::__construct($arguments);

    // Migration source settings.
    $csv_path = drupal_get_path('module', 'd7_example_content') . '/migrate/content/nodes/basic_page.csv';
    $columns = $this->csvcolumns();
    $content_type = 'test'; // Machine name of the content type

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->description = t('Import: Basic Page');
    $this->source = new MigrateSourceCSV($csv_path, $columns, array('header_rows' => 1));
    $this->destination = new MigrateDestinationNode($content_type);

    // Source and destination relation for rollbacks
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Topic ID',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Set defaults for unmapped fields
    $this->addFieldMapping('promote')->defaultValue(0);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);

    // Mappings
    $this->addSimpleMappings(array('title', 'body', 'path'));
    $this->addFieldMapping('body:format', 'format')->defaultValue('filtered_html');
  }

  // CVS column mapping.
  function csvcolumns() {
    // title, body, format, path, enable_menu, menu_name, menu_weight
    $cols[0] = array('title', 'Node title');
    $cols[1] = array('body', 'Body text');
    $cols[2] = array('path', 'Node path');
    $cols[3] = array('enable_menu', 'Menu status');
    $cols[4] = array('menu_name', 'Menu name');
    $cols[5] = array('menu_weight', 'Menu weight');
    $cols[6] = array('menu_parent', 'Menu parent');
    return $cols;
  }

  // Alter the results based on the CSV row.
  public function prepare($entity, $row) {
    /**
     * Create a menu item.
     */
    if ($row->enable_menu) {
      // See if our content is going to be a child item.
      $plid = 0;
      if ($parent = $row->menu_parent) {
        $links = menu_load_links($row->menu_name);
        foreach ($links as $link) {
          if ($link['link_path'] == drupal_get_normal_path($parent)) {
            $plid = $link['mlid'];
          }
        }
      }
      // Create our menu item
      $menu = array(
        'enabled' => 1,
        'link_title' => $row->title,
        'plid' => $plid,
        'menu_name' => $row->menu_name,
        'weight' => $row->menu_weight,
        'description' => '',
      );
      $entity->menu = array_merge($entity->menu, $menu);
    }
  }
}
