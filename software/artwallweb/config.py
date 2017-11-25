from collections import namedtuple

Project = namedtuple('Project','name, lightgroups')
LightGroup = namedtuple('LightGroup','name, connection, num_boards, lights')
Light = namedtuple('Light','name, board_num, index, channels, loc')

project=Project(name='Art-Wall',lightgroups={})

project.lightgroups['moon']=LightGroup(name='moon',
                                      connection='usb',
                                      num_boards=2,
                                      lights=[])

project.lightgroups['moon'].lights.append(Light(name='left',
                      board_num=0,
                      channels=[0,1,2],
                      index=0,
                      loc=((0,0),(2,2))))
    


if __name__ == '__main__':
    import pprint
    pprint.pprint(project)
    